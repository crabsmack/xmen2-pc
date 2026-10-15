"""Phase-2 and phase-4 contract tool (xmen2disasm docs/phases/P2-recomp.md, P4-decomp.md):
report what the build and the runner actually did. Never fills gaps: every number comes from
the recompiler's or the runner's own output, or from reading the hand-written C.

    uv run tools/xmen2check.py gen   [--json]   # regenerate C into build/gen-check/, count problems
    uv run tools/xmen2check.py build [--json]   # configure + build the Release runners
    uv run tools/xmen2check.py trace --run NAME --input FILE.input --out DIR [--frames N]
                                     [--mode recomp [--sync] | stepped --replay REFDIR] [--json]
    uv run tools/xmen2check.py types|handc|lint [--json]   # phase 4: the hand-written C itself

gen:   {"unsupported": N, "validator_errors": N, "functions": ["0BA4A4", ...], ...}
       unsupported = the recompiler's own "Unsupported summary" TOTAL (TODO/stub emissions,
       misaligned entries, EA fallbacks), validator_errors = speculative paths the validator
       terminated, functions = every func_XXXXXX the generated header declares.
build: {"exe": "...", "config": "Release", "ok": true}
trace: plays FILE.input headless and uncapped on a private copy of the Release runner
       (6-button pads on both ports, like the reference); writes DIR/frames.csv
       (frame,ram_crc32,vram_crc32,cram_crc32,vsram_crc32,z80_crc32 after every frame, from the
       game spec's on_frame_post) and prints
       {"frames", "dispatch_misses", "interp_instructions", "stub_hits", "fps", "exe"}
       from the runner's own counters (raw dispatch-miss events, cumulative Tier-3
       interpreter instructions, dispatch-override hits).
"""
from __future__ import annotations

import argparse
import json
import re
import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

BUILD = common.BUILD
GENERATOR = "Visual Studio 17 2022"


def log(msg: str) -> None:
    print(msg, file=sys.stderr, flush=True)


def cmake(*args: str, logfile: Path) -> int:
    with open(logfile, "w", encoding="utf-8", errors="replace") as f:
        return subprocess.run([common.tool("cmake"), *args], stdout=f, stderr=subprocess.STDOUT).returncode


def configure() -> None:
    BUILD.mkdir(exist_ok=True)
    if (BUILD / "CMakeCache.txt").exists():
        return
    args = ["-S", str(common.ROOT), "-B", str(BUILD)]
    if sys.platform == "win32":
        args += ["-G", GENERATOR, "-A", "x64"]
    if cmake(*args, logfile=BUILD / "configure.log"):
        sys.exit(f"configure failed: {BUILD / 'configure.log'}")


STEPPED_EXE = "XMen2Stepped.exe" if sys.platform == "win32" else "XMen2Stepped"


def exe_path(name: str = common.EXE_NAME) -> Path:
    return BUILD / "Release" / name if sys.platform == "win32" else BUILD / name


# --- gen -------------------------------------------------------------------------------

def cmd_gen(a) -> dict:
    common.ensure_rom()
    configure()
    if cmake("--build", str(BUILD), "--config", "Release", "--target", "GenesisRecomp",
             logfile=BUILD / "recompiler.log"):
        sys.exit(f"recompiler build failed: {BUILD / 'recompiler.log'}")
    rec = next((p for p in BUILD.rglob("GenesisRecomp.exe" if sys.platform == "win32" else "GenesisRecomp")
                if p.is_file()), None)
    if not rec:
        sys.exit("GenesisRecomp executable not found under build/")
    out = BUILD / "gen-check"
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True)
    r = subprocess.run([str(rec), "xmen2.bin", "--game", "game.toml", "--output-dir", str(out)],
                       cwd=common.GAME, capture_output=True, text=True, errors="replace")
    text = r.stdout + r.stderr
    (out / "recompiler.log").write_text(text, encoding="utf-8")
    if r.returncode != 0:
        sys.exit(f"recompiler failed ({r.returncode}): {out / 'recompiler.log'}")
    m = re.search(r"Unsupported summary:.*?TOTAL\s+(\d+)", text, re.S)
    v = re.search(r"(\d+) speculative paths terminated by validator", text)
    decls = (out / "xmen2_decls.h").read_text(errors="replace")
    funcs = sorted(set(re.findall(r"\bvoid func_([0-9A-F]{6})\(void\)", decls)))
    audit = (out / "xmen2_dispatch_audit.log").read_text(errors="replace") if (out / "xmen2_dispatch_audit.log").exists() else ""
    hyb = re.search(r"Dispatch audit: (\d+) sites \(.*?(\d+) hybrid", text)
    return {"unsupported": int(m.group(1)) if m else None,
            "validator_errors": int(v.group(1)) if v else None,
            "functions": funcs,
            "function_count": len(funcs),
            "hybrid_dispatch_sites": int(hyb.group(2)) if hyb else None,
            "interior_unresolved": int(re.search(r"INTERIOR_UNRESOLVED\s+(\d+)", audit).group(1))
            if "INTERIOR_UNRESOLVED" in audit else None,
            "log": str(out / "recompiler.log")}


# --- build -----------------------------------------------------------------------------

def cmd_build(a) -> dict:
    common.ensure_rom()
    # the bridge between hand-written C and generated code (build/bridge), before cmake reads it
    for mode, d in (("stepped", "bridge"), ("verify", "bridge_verify")):
        r = subprocess.run([sys.executable, str(common.ROOT / "tools" / "bridge.py"), "--out", str(BUILD / d),
                            "--mode", mode], capture_output=True, text=True)
        if r.returncode:
            sys.exit(f"bridge.py failed: {(r.stdout + r.stderr).strip()[-400:]}")
    configure()
    rc = cmake("--build", str(BUILD), "--config", "Release", "--target", "XMen2Recomp", "XMen2Stepped", "XMen2Verify",
               "--parallel", logfile=BUILD / "build.log")
    exe = exe_path()
    stepped, verify = exe_path(STEPPED_EXE), exe_path(VERIFY_EXE)
    return {"exe": str(exe), "stepped_exe": str(stepped), "verify_exe": str(verify), "config": "Release",
            "ok": rc == 0 and exe.is_file() and stepped.is_file() and verify.is_file(),
            "log": str(BUILD / "build.log")}


# --- trace -----------------------------------------------------------------------------

SETTINGS = """[input.p1]
device = 0
pad_type = 1

[input.p2]
device = 0
pad_type = 1
"""


def script_frames(path: Path) -> int:
    """Frames an exported .input covers: the sum of its WAITs."""
    return sum(int(m.group(1), 0) for m in re.finditer(r"^WAIT\s+(\S+)", path.read_text(), re.M))


def csv_rows(p: Path) -> int:
    return sum(1 for _ in open(p)) - 1 if p.exists() else 0


def cmd_trace(a) -> dict:
    """recomp: the timed runner plays the input (phase 2); --sync adds sync.csv and the
    replay log (phase 4 D1). stepped: the untimed runner replays REFDIR/replay.bin and
    writes sync.csv (phase 4 D2); no input script, the pads come from the log."""
    import os
    if a.mode not in ("recomp", "stepped"):
        sys.exit(f"--mode {a.mode}: not implemented (recomp, stepped)")
    if a.mode == "stepped" and not a.replay:
        sys.exit("--mode stepped needs --replay REFDIR (a `--mode recomp --sync` trace)")
    common.ensure_rom()
    src = exe_path(STEPPED_EXE if a.mode == "stepped" else common.EXE_NAME)
    if not src.is_file():
        sys.exit(f"no Release runner at {src}: run `xmen2check.py build`")
    out = Path(a.out).resolve()
    out.mkdir(parents=True, exist_ok=True)
    run_dir = out / "runner"  # private copy: settings.ini and evidence files stay per trace
    if run_dir.exists():
        shutil.rmtree(run_dir)
    run_dir.mkdir()
    for f in src.parent.iterdir():
        if f.is_file() and f.suffix.lower() in (".exe", ".dll", ".pdb", ""):
            shutil.copy2(f, run_dir / f.name)
    (run_dir / "settings.ini").write_text(SETTINGS)
    env = {**os.environ, "GENESIS_NO_LAUNCHER": "1", "SDL_AUDIODRIVER": "dummy"}
    sync_csv = out / "sync.csv"
    if a.mode == "stepped":
        ref = Path(a.replay).resolve()
        if not (ref / "replay.bin").is_file() or not (ref / "sync.csv").is_file():
            sys.exit(f"{ref} has no replay.bin + sync.csv (a `trace --mode recomp --sync` output)")
        # One runner frame per sync interval; the run stops itself where the reference ended.
        frames = csv_rows(ref / "sync.csv") + 2
        env.update(GENESIS_REPLAY_IN=str(ref / "replay.bin"), XMEN2_SYNC_CSV=str(sync_csv))
        cmd = [str(run_dir / src.name), str(common.ROM), "--benchmark", str(frames), "--fresh-evidence"]
    else:
        frames = a.frames or script_frames(Path(a.input))
        # --benchmark N ends the run after N frames and prints the counters; an EXIT directive
        # would return from the runner before that summary, so the played copy drops it.
        script = out / "input.input"
        script.write_text("".join(l for l in Path(a.input).read_text().splitlines(True)
                                  if not l.strip().upper().startswith("EXIT")), newline="\n")
        env["XMEN2_TRACE_CSV"] = str(out / "frames.csv")
        if a.sync:
            env.update(XMEN2_SYNC_CSV=str(sync_csv), GENESIS_REPLAY_OUT=str(out / "replay.bin"))
        if a.dump_frames:  # first-divergence state dumps (game/xmen2_spec.c maybe_dump)
            (out / "dump").mkdir(exist_ok=True)
            env.update(XMEN2_DUMP_FRAMES=a.dump_frames, XMEN2_DUMP_DIR=str(out / "dump"))
        cmd = [str(run_dir / src.name), str(common.ROM), "--benchmark", str(frames),
               "--input-script", str(script), "--fresh-evidence"]
    log(f"trace {a.run} ({a.mode}{', sync' if a.sync else ''}): {frames} frames")
    with open(out / "runner.log", "w", encoding="utf-8", errors="replace") as lf:
        r = subprocess.run(cmd, cwd=run_dir, env=env, stdout=subprocess.PIPE, stderr=lf, text=True,
                           errors="replace", timeout=a.timeout)
    (out / "runner.stdout").write_text(r.stdout, encoding="utf-8")
    stderr = (out / "runner.log").read_text(errors="replace")
    bench = re.search(r"GENESISRECOMP_BENCHMARK (\{.*\})", r.stdout)
    counters = re.search(r"\[xmen2\] counters (\{.*\})", stderr)
    div = re.search(r"\[replay\] divergence.*", stderr)
    if div:
        sys.exit(f"replay {div.group(0)} (see {out / 'runner.log'})")
    if r.returncode != 0 or not bench or not counters:
        sys.exit(f"runner failed ({r.returncode}); see {out / 'runner.log'}")
    b, c = json.loads(bench.group(1)), json.loads(counters.group(1))
    res = {"dispatch_misses": c["dispatch_misses"], "interp_instructions": c["interp_instructions"],
           "stub_hits": c["stub_hits"], "fps": b["fps"], "exe": str(run_dir / src.name)}
    if a.mode == "stepped":
        res["sync_points"] = csv_rows(sync_csv)
        res["replay_complete"] = "[replay] end of the reference" in stderr
        return res
    res.update(frames=csv_rows(out / "frames.csv"), complete=b["complete"])
    if a.sync:
        # stack_low: the fixed bound the sync CRCs mask from (the corpus's lowest A7);
        # reported only if this run never went below it (else the CRCs are not valid).
        a7 = c.get("a7_min")
        low = c.get("stack_low")
        res.update(sync_points=csv_rows(sync_csv), a7_min=a7,
                   stack_low=low if a7 is not None and low is not None and a7 >= low else None)
    return res


# --- phase 4: verify (shadow differential tests) --------------------------------------

VERIFY_EXE = "XMen2Verify.exe" if sys.platform == "win32" else "XMen2Verify"


def code_addrs_by_function() -> dict[str, list[int]]:
    """Every instruction start of each function's own range (game/xmen2.*)."""
    import bisect
    import csv as _csv
    with open(common.GAME / "xmen2.functions.csv", newline="") as f:
        funcs = sorted((int(r["addr"], 16), int(r["end"], 16), r["addr"]) for r in _csv.DictReader(f))
    starts = [s for s, _e, _a in funcs]
    out: dict[str, list[int]] = {a: [] for _s, _e, a in funcs}
    for tok in (common.GAME / "xmen2.code_addrs.txt").read_text().split():
        if tok.startswith("#"):
            continue
        pc = int(tok, 16)
        i = bisect.bisect_right(starts, pc) - 1
        if i >= 0 and pc < funcs[i][1]:
            out[funcs[i][2]].append(pc)
    return out


def cmd_verify(a) -> dict:
    """Play every corpus input on the verification runner (all generated code, timed, with the
    hand-written C as shadow tests: game/runtime/verify.cpp) and merge what the runs report."""
    import os
    common.ensure_rom()
    src = exe_path(VERIFY_EXE)
    if not src.is_file():
        sys.exit(f"no verification runner at {src}: run `xmen2check.py build`")
    inputs = sorted(Path(a.inputs).glob("*.input"))
    if a.runs:
        keep = set(a.runs.split(","))
        inputs = [p for p in inputs if p.stem in keep]
    if not inputs:
        sys.exit(f"no <run>.input files in {a.inputs}")
    out = BUILD / "verify"
    out.mkdir(parents=True, exist_ok=True)
    run_dir = out / "runner"
    if run_dir.exists():
        shutil.rmtree(run_dir)
    run_dir.mkdir()
    for f in src.parent.iterdir():
        if f.is_file() and f.suffix.lower() in (".exe", ".dll", ".pdb", ""):
            shutil.copy2(f, run_dir / f.name)
    (run_dir / "settings.ini").write_text(SETTINGS)
    merged: dict[str, dict] = {}
    for inp in inputs:
        run = inp.stem
        script = out / f"{run}.input"
        script.write_text("".join(l for l in inp.read_text().splitlines(True)
                                  if not l.strip().upper().startswith("EXIT")), newline="\n")
        res = out / f"{run}.json"
        if res.exists():
            res.unlink()
        env = {**os.environ, "GENESIS_NO_LAUNCHER": "1", "SDL_AUDIODRIVER": "dummy", "XMEN2_VERIFY_OUT": str(res)}
        if a.functions:
            env["XMEN2_VERIFY_ONLY"] = a.functions
        frames = a.frames or script_frames(inp)
        cmd = [str(run_dir / src.name), str(common.ROM), "--benchmark", str(frames),
               "--input-script", str(script), "--fresh-evidence"]
        log(f"verify {run}: {frames} frames")
        with open(out / f"{run}.log", "w", encoding="utf-8", errors="replace") as lf:
            r = subprocess.run(cmd, cwd=run_dir, env=env, stdout=subprocess.DEVNULL, stderr=lf, timeout=a.timeout)
        if r.returncode != 0 or not res.exists():
            sys.exit(f"verification runner failed on {run} ({r.returncode}); see {out / (run + '.log')}")
        data = json.loads(res.read_text())
        for addr, v in data["functions"].items():
            m = merged.setdefault(addr, {"calls_checked": 0, "mismatches": 0, "irq_skipped": 0,
                                         "first_mismatch": None, "covered": set()})
            m["calls_checked"] += v["calls_checked"]
            m["mismatches"] += v["mismatches"]
            m["irq_skipped"] += v["irq_skipped"]
            if v["first_mismatch"] and not m["first_mismatch"]:
                m["first_mismatch"] = f"{run}: {v['first_mismatch']}"
            m["covered"].update(v["covered"])
    insns = code_addrs_by_function()
    funcs = {}
    for addr, m in sorted(merged.items()):
        own = insns.get(addr, [])
        unc = [pc for pc in own if pc not in m["covered"]]
        funcs[addr] = {"calls_checked": m["calls_checked"], "mismatches": m["mismatches"],
                       "first_mismatch": m["first_mismatch"], "irq_skipped": m["irq_skipped"],
                       "instructions": len(own), "uncovered": len(unc),
                       "first_uncovered": f"{unc[0]:06X}" if unc else None}
    return {"runs": len(inputs), "functions": funcs}


# --- phase 4: the hand-written C (types, handc, lint) ----------------------------------

INC = common.GAME / "include"
CDIR = common.GAME / "c"
# The memory/hardware layer: the only headers that may name addresses.
MEMORY_LAYER = {"xmen2_mem.h", "xmen2_globals.h", "xmen2_data.h"}


def strip_comments(t: str) -> str:
    """C comments -> spaces (newlines kept, so line numbers stay); strings left as they are."""
    return re.sub(r"/\*.*?\*/", lambda m: re.sub(r"[^\n]", " ", m.group(0)), t, flags=re.S)


def decl_names(body: str) -> list[str]:
    """The names declared by the field declarations in a struct body / FIELDS macro."""
    names = []
    for stmt in body.replace("\\\n", "\n").split(";"):
        s = " ".join(stmt.split())
        if not s or re.fullmatch(r"[A-Z0-9_]+_FIELDS(\s+[A-Z0-9_]+_FIELDS)*", s):
            continue
        s = re.sub(r"^[A-Z0-9_]+_FIELDS\s+", "", s)   # FIELDS macro followed by a field
        s = re.sub(r"\[[^\]]*\]", "", s)              # array bounds
        m = re.search(r"([A-Za-z_]\w*)\s*(\)\s*\([^)]*\))?\s*$", s)
        if m:
            names.append(m.group(1))
    return names


def cmd_types(a) -> dict:
    structs, fields = set(), []
    for h in sorted(INC.glob("*.h")):
        t = strip_comments(h.read_text(encoding="utf-8"))
        for m in re.finditer(r"\bstruct\s+([A-Za-z_]\w*)\s*\{([^{}]*)\}", t):
            structs.add(m.group(1))
            fields += decl_names(m.group(2))
        for m in re.finditer(r"^#define\s+[A-Z0-9_]+_FIELDS\s*\\\n((?:.*\\\n)*.*)$", t, re.M):
            fields += decl_names(m.group(1))
    glob_t = strip_comments((INC / "xmen2_globals.h").read_text(encoding="utf-8")) if (INC / "xmen2_globals.h").exists() else ""
    globals_ = sorted(set(re.findall(r"^extern\s+[^;]*?\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*;", glob_t, re.M)))
    return {"structs": sorted(structs), "globals": globals_, "fields": len(fields),
            "unk_fields": sum(1 for f in fields if f.startswith("unk_"))}


DEF_RE = re.compile(r"^[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\(([^;{]*)\)\s*\{", re.M)


def cmd_handc(a) -> dict:
    out = {}
    for p in sorted(CDIR.glob("*.c")) if CDIR.exists() else []:
        if not re.fullmatch(r"[0-9A-Fa-f]{6}", p.stem):
            continue
        raw = p.read_text(encoding="utf-8")
        code = strip_comments(raw)
        defs = [m.group(1) for m in DEF_RE.finditer(code) if m.group(1) not in ("if", "while", "for", "switch")]
        name = defs[0] if defs else None
        # the house style: a block comment "Name - what it does" before the definition
        doc = bool(name) and bool(re.search(r"/\*\s*" + re.escape(name) + r"\s+-\s+\S", raw[:raw.find(name + "(")]
                                            if name + "(" in raw else ""))
        out[p.stem.upper()] = {"name": name, "doc": doc, "file": f"game/c/{p.name}", "defines": defs}
    return {"functions": out}


LINT_RULES = ("registers", "raw_addresses", "raw_offsets", "cross_function_goto")
HW_OR_RAM = re.compile(r"\b0x(?:00)?(A[01][0-9A-F]{4}|C000[01][0-9A-F]|F[F][0-9A-F]{4}|E[0-9A-F]{5})\b", re.I)
PTR_CAST_LIT = re.compile(r"\(\s*(?:volatile\s+|const\s+)*[A-Za-z_]\w*\s*\*+\s*\)\s*\(?\s*(?:0x[0-9A-Fa-f]+|\d+)\b")
BYTE_PTR_OFF = re.compile(r"\(\s*(?:const\s+)?(?:char|u8|s8|unsigned\s+char|signed\s+char|void)\s*\*\s*\)\s*"
                          r"\(?\s*[A-Za-z_][\w>.\-]*\s*\)?\s*\)?\s*[+\-]\s*(?:0x[0-9A-Fa-f]+|\d+)")
BYTE_PTR_IDX = re.compile(r"\(\s*\(\s*(?:const\s+)?(?:char|u8|s8|unsigned\s+char|signed\s+char)\s*\*\s*\)\s*"
                          r"[A-Za-z_][\w>.\-]*\s*\)\s*\[")


def lint_text(name: str, t: str) -> dict[str, int]:
    code = strip_comments(t)
    code = re.sub(r'"(?:\\.|[^"\\])*"', '""', code)   # string contents are not code
    v = dict.fromkeys(LINT_RULES, 0)
    v["registers"] = len(re.findall(r"\bg_cpu\b|\bXI\s*\(|\bXC\s*\(|\bXSR\s*\(|\bg_xrts\b", code))
    if name not in MEMORY_LAYER:
        v["raw_addresses"] = (len(HW_OR_RAM.findall(code)) + len(PTR_CAST_LIT.findall(code))
                              + len(re.findall(r"\bm68k_(?:read|write)\w*\s*\(|\bxm::(?:host|rom)\s*\(|\bg_ram\w*\b|\bg_rom\b", code)))
        v["raw_offsets"] = len(BYTE_PTR_OFF.findall(code)) + len(BYTE_PTR_IDX.findall(code))
    v["cross_function_goto"] = len(re.findall(r"\b(?:setjmp|longjmp)\s*\(|\bfunc_[0-9A-F]{6}\b|\brecomp_\w+\s*\(", code))
    return v


def cmd_lint(a) -> dict:
    files = {}
    for p in sorted(CDIR.glob("*.c")) if CDIR.exists() else []:
        files[f"game/c/{p.name}"] = lint_text(p.name, p.read_text(encoding="utf-8"))
    for p in sorted(INC.glob("*.h")):
        files[f"game/include/{p.name}"] = lint_text(p.name, p.read_text(encoding="utf-8"))
    total = dict.fromkeys(LINT_RULES, 0)
    for v in files.values():
        for k in LINT_RULES:
            total[k] += v[k]
    return {"violations": total, "files": files}


def main() -> int:
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name in ("types", "handc", "lint"):
        p = sub.add_parser(name)
        p.add_argument("--json", action="store_true")
    p = sub.add_parser("verify")
    p.add_argument("--json", action="store_true")
    p.add_argument("--inputs", required=True, help="dir with <run>.input for every corpus run")
    p.add_argument("--functions", default="", help="only these (comma-separated addresses)")
    p.add_argument("--runs", default="", help="dev: only these runs (comma-separated)")
    p.add_argument("--frames", type=int, default=0, help="dev: only the first N frames of each run")
    p.add_argument("--timeout", type=int, default=7200)
    for name in ("gen", "build", "trace"):
        p = sub.add_parser(name)
        p.add_argument("--json", action="store_true")
        if name == "trace":
            p.add_argument("--run", required=True)
            p.add_argument("--input", default="")
            p.add_argument("--out", required=True)
            p.add_argument("--frames", type=int, default=0)
            p.add_argument("--mode", default="recomp")
            p.add_argument("--sync", action="store_true", help="recomp: also sync.csv + replay.bin (P4 D1)")
            p.add_argument("--replay", default="", help="stepped: the D1 trace dir to replay")
            p.add_argument("--timeout", type=int, default=7200)
            p.add_argument("--dump-frames", default="", help="dev: dump full state after these frames")
    a = ap.parse_args()
    res = {"gen": cmd_gen, "build": cmd_build, "trace": cmd_trace, "types": cmd_types,
           "handc": cmd_handc, "lint": cmd_lint, "verify": cmd_verify}[a.cmd](a)
    if a.cmd == "gen" and not a.json:
        res = {k: v for k, v in res.items() if k != "functions"}
    print(json.dumps(res, indent=None if a.json else 1))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
