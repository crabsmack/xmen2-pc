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
STEPPED_GEN_EXE = "XMen2SteppedGen.exe" if sys.platform == "win32" else "XMen2SteppedGen"


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

def snapshot_handc() -> Path:
    """build/handc_src: game/c/*.c and game/include/*.h as they are now, in one directory, which
    the bridges and the hand-C compile read (C being edited meanwhile can't mix two states into
    one build). Unchanged files keep their timestamps, so only edits recompile."""
    import shutil
    snap = BUILD / "handc_src"
    snap.mkdir(parents=True, exist_ok=True)
    want = {p.name: p for p in list((common.GAME / "c").glob("*.c")) + list((common.GAME / "include").glob("*.h"))}
    for p in snap.iterdir():
        if p.name not in want:
            p.unlink()
    for name, src in want.items():
        dst = snap / name
        data = src.read_bytes()
        if not dst.exists() or dst.read_bytes() != data:
            dst.write_bytes(data)
    return snap


def cmd_build(a) -> dict:
    common.ensure_rom()
    snap = snapshot_handc()
    # the bridge between hand-written C and generated code (build/bridge), before cmake reads it
    for mode, d in (("stepped", "bridge"), ("verify", "bridge_verify")):
        r = subprocess.run([sys.executable, str(common.ROOT / "tools" / "bridge.py"), "--out", str(BUILD / d),
                            "--mode", mode, "--src", str(snap)], capture_output=True, text=True)
        if r.returncode:
            sys.exit(f"bridge.py failed: {(r.stdout + r.stderr).strip()[-400:]}")
    configure()
    rc = cmake("--build", str(BUILD), "--config", "Release", "--target", "XMen2Recomp", "XMen2Stepped",
               "XMen2SteppedGen", "XMen2Verify", "--parallel", logfile=BUILD / "build.log")
    exes = {"exe": exe_path(), "stepped_exe": exe_path(STEPPED_EXE), "stepped_gen_exe": exe_path(STEPPED_GEN_EXE),
            "verify_exe": exe_path(VERIFY_EXE)}
    res = {**{k: str(v) for k, v in exes.items()}, "config": "Release",
           "ok": rc == 0 and all(v.is_file() for v in exes.values()), "log": str(BUILD / "build.log")}
    if a.handc:
        # The hand-C build is the stepped runner: the recompiler leaves every function written in
        # C out of its code (--skip). Count the game functions (the disassembly's) whose generated
        # code it still links: a func_ADDR / func_body_ADDR definition in its generated C.
        with open(common.GAME / "xmen2.functions.csv", newline="") as f:
            game = {r["addr"].upper() for r in __import__("csv").DictReader(f)}
        defined = set()
        for p in sorted((BUILD / "generated" / "xmen2s").glob("xmen2s_part*.c")):
            defined.update(m.group(1) for m in re.finditer(
                r"^(?:static )?void func_(?:body_)?([0-9A-F]{6})\((?:void|uint32_t _entry)\) \{", p.read_text(errors="replace"), re.M))
        gen = sorted(game & defined)
        res.update(exe=str(exes["stepped_exe"]), generated_game_functions=len(gen), first=gen[0] if gen else None)
    return res


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


def input_pads(path: Path) -> list[str]:
    """The pads of every frame of an exported .input (index 0 = frame 1): 'p1 p2'."""
    pads, cur = [], "0 0"
    for line in path.read_text().splitlines():
        w = line.split()
        if not w or w[0].startswith("#"):
            continue
        if w[0].upper() == "PADS":
            cur = f"{w[1]} {w[2]}"
        elif w[0].upper() == "WAIT":
            pads += [cur] * int(w[1], 0)
    return pads


def sync_frames(ref: Path) -> list[int]:
    import csv as _csv
    with open(ref / "sync.csv", newline="") as f:
        return [int(r["frame"]) for r in _csv.DictReader(f)]


def stepped_script(inp: Path, ref: Path, out: Path) -> int:
    """The frame-synchronous runner's input: one runner frame per sync interval. Frame 1 (boot
    to sync 1) gets the input's first frame; frame k+1 (after sync k) the input of the
    reference frame of sync k (D1 sync.csv), the frame whose V-int read the pads. Returns the
    number of runner frames (= the reference's sync points)."""
    pads, frames = input_pads(inp), sync_frames(ref)
    seq = [pads[0] if pads else "0 0"] + [pads[min(f, len(pads)) - 1] if pads else "0 0" for f in frames[:-1]]
    lines, prev, n = [], None, 0
    for p in seq:
        if p != prev and prev is not None:
            lines += [f"PADS {prev}", f"WAIT {n}"]
            n = 0
        prev, n = p, n + 1
    if prev is not None:
        lines += [f"PADS {prev}", f"WAIT {n}"]
    out.write_text("# per sync interval (xmen2check.py trace --mode stepped)\n" + "\n".join(lines) + "\n", newline="\n")
    return len(frames)


STEPPED_MODES = {"stepped": "STEPPED_EXE", "stepped-gen": "STEPPED_GEN_EXE", "handc": "STEPPED_EXE"}


def cmd_trace(a) -> dict:
    """recomp: the timed runner plays the input (phase 2); --sync adds sync.csv and the
    replay log (phase 4 D1). stepped / stepped-gen: the frame-synchronous untimed runner, with
    hand C / with all generated code, plays the input one sync interval per runner frame (the
    pads of each sync point's reference frame, from REFDIR = the run's D1 trace) and writes
    sync.csv (phase 4 D2: the two must be equal)."""
    import os
    if a.mode not in ("recomp", *STEPPED_MODES):
        sys.exit(f"--mode {a.mode}: not implemented (recomp, {', '.join(STEPPED_MODES)})")
    if a.mode in STEPPED_MODES and not a.replay:
        sys.exit(f"--mode {a.mode} needs --replay REFDIR (the run's `--mode recomp --sync` trace)")
    common.ensure_rom()
    src = exe_path(globals()[STEPPED_MODES[a.mode]] if a.mode in STEPPED_MODES else common.EXE_NAME)
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
    if a.mode in STEPPED_MODES:
        ref = Path(a.replay).resolve()
        if not (ref / "sync.csv").is_file():
            sys.exit(f"{ref} has no sync.csv (a `trace --mode recomp --sync` output)")
        if not a.input:
            sys.exit(f"--mode {a.mode} needs --input (the run's exported .input)")
        script = out / "input.input"
        frames = stepped_script(Path(a.input), ref, script)
        if a.frames:
            frames = min(frames, a.frames)
        env.update(XMEN2_SYNC_CSV=str(sync_csv))
        env.pop("GENESIS_REPLAY_IN", None)
        cmd = [str(run_dir / src.name), str(common.ROM), "--benchmark", str(frames),
               "--input-script", str(script), "--fresh-evidence"]
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
    if a.mode in STEPPED_MODES:
        res["sync_points"] = csv_rows(sync_csv)
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
    import concurrent.futures
    import queue
    out = BUILD / "verify"
    out.mkdir(parents=True, exist_ok=True)
    jobs = max(1, min(a.jobs, len(inputs)))
    free: "queue.Queue[Path]" = queue.Queue()
    for k in range(jobs):   # one runner copy per worker: settings.ini and evidence files stay apart
        run_dir = out / f"runner{k}"
        if run_dir.exists():
            shutil.rmtree(run_dir)
        run_dir.mkdir()
        for f in src.parent.iterdir():
            if f.is_file() and f.suffix.lower() in (".exe", ".dll", ".pdb", ""):
                shutil.copy2(f, run_dir / f.name)
        (run_dir / "settings.ini").write_text(SETTINGS)
        free.put(run_dir)
    # each function's instruction count: verify.cpp probes the calls its sampling skips until
    # the checked calls have covered them all
    insns = out / "insns.txt"
    insns.write_text("".join(f"{addr} {len(pcs)}\n" for addr, pcs in sorted(code_addrs_by_function().items())))

    def one(inp: Path):
        run = inp.stem
        script = out / f"{run}.input"
        script.write_text("".join(l for l in inp.read_text().splitlines(True)
                                  if not l.strip().upper().startswith("EXIT")), newline="\n")
        res = out / f"{run}.json"
        if res.exists():
            res.unlink()
        # synthetic cases (game/verify/<ADDR>.syn; verify.cpp load_synth)
        env = {**os.environ, "GENESIS_NO_LAUNCHER": "1", "SDL_AUDIODRIVER": "dummy", "XMEN2_VERIFY_OUT": str(res),
               "XMEN2_VERIFY_SYNTH": str(common.GAME / "verify"), "XMEN2_VERIFY_INSNS": str(insns)}
        if a.functions:
            env["XMEN2_VERIFY_ONLY"] = a.functions
        frames = a.frames or script_frames(inp)
        cmd = [src.name, str(common.ROM), "--benchmark", str(frames),
               "--input-script", str(script), "--fresh-evidence"]
        run_dir = free.get()
        try:
            cmd[0] = str(run_dir / src.name)
            log(f"verify {run}: {frames} frames")
            with open(out / f"{run}.log", "w", encoding="utf-8", errors="replace") as lf:
                r = subprocess.run(cmd, cwd=run_dir, env=env, stdout=subprocess.DEVNULL, stderr=lf, timeout=a.timeout)
        finally:
            free.put(run_dir)
        if r.returncode != 0 or not res.exists():
            return run, None, f"verification runner failed on {run} ({r.returncode}); see {out / (run + '.log')}"
        return run, json.loads(res.read_text()), None

    merged: dict[str, dict] = {}
    # the longest runs start first (the TAS, then the late fuzz runs: their TAS prefix), so the
    # last ones to finish are short; the merge stays in run order
    longest = sorted(inputs, key=lambda p: -(a.frames or script_frames(p)))
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
        done = {p: f for p, f in zip(longest, [ex.submit(one, p) for p in longest])}
        results = [done[p].result() for p in inputs]
    for run, data, err in results:   # in run order: the same merge as a serial run
        if err:
            sys.exit(err)
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
        if 0 < len(unc) <= 64:   # for P4-decomp.md decisions 10, 13 (listed unreachable instructions)
            funcs[addr]["uncovered_pcs"] = [f"{pc:06X}" for pc in unc]
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
    # P4-decomp.md decisions 7 and 8: fragments (blocks the disassembly split off a C function,
    # and AI-script thunk tables) are done with the C that contains / names them; asm functions
    # (no MPW frame) are reported for the never-executed rule.
    tags = {}
    whole_game = []
    for line in (INC / "xmen2_funcs.h").read_text(encoding="utf-8").splitlines():
        # the first word of the tag: `asm`, `fragment`... (an asm tag may go on: `asm regs(d0)->d0`)
        m = re.search(r"\b([A-Za-z_]\w*)\s*\([^;]*\)\s*;\s*/\*\s*\$([0-9A-F]{6})\s*((\w+)?[^*]*)\*/", line)
        if m:
            tags[m.group(2)] = (m.group(1), m.group(4) or "")
            # decision 12: hand-written 68000 that never returns or ends in rte (no match, no
            # shadow test) is verified by the whole-game comparisons
            if m.group(4) == "asm" and re.search(r"\b(noreturn|vector)\b", m.group(3)):
                whole_game.append(m.group(2))
    texts = {a: (CDIR / f"{a}.c").read_text(encoding="utf-8") for a in out}
    order = sorted(tags)
    frags = {}
    for addr, (name, tag) in tags.items():
        if tag != "fragment":
            continue
        named_in = sorted(a for a, t in texts.items() if re.search(rf"\b{re.escape(name)}\b", t))
        parent = named_in[0] if named_in else None
        if parent is None:   # the C function the block was split from: the nearest one before it
            i = order.index(addr)
            parent = next((order[j] for j in range(i - 1, -1, -1) if tags[order[j]][1] != "fragment"), None)
        frags[addr] = {"name": name, "parent": parent, "named": bool(named_in)}
    return {"functions": out, "fragments": frags,
            "asm": sorted(a for a, (_n, t) in tags.items() if t == "asm"), "whole_game": sorted(whole_game)}


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
    p.add_argument("--jobs", type=int, default=24,
                   help="runs played at once (each on its own runner copy; ~80 MB and a fraction of a core each)")
    p.add_argument("--timeout", type=int, default=7200)
    for name in ("gen", "build", "trace"):
        p = sub.add_parser(name)
        p.add_argument("--json", action="store_true")
        if name == "build":
            p.add_argument("--handc", action="store_true", help="P4 D6: count generated game functions linked")
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
