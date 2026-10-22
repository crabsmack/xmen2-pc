"""Phase-2, phase-4 and phase-5 contract tool (xmen2disasm docs/phases/P2-recomp.md,
P4-decomp.md, P5-port.md): report what the build and the runner actually did. Never fills gaps:
every number comes from the recompiler's or the runner's own output, from the object files and
link map the build wrote, or from reading the hand-written C.

    uv run tools/xmen2check.py gen   [--json]   # regenerate C into build/gen-check/, count problems
    uv run tools/xmen2check.py build [--json]   # configure + build the Release runners
    uv run tools/xmen2check.py build --native [--json]   # + what each object still needs from the 68000
    uv run tools/xmen2check.py trace --run NAME --input FILE.input --out DIR [--frames N]
                                     [--mode recomp [--sync] | stepped|stepped-gen|handc|native
                                      --replay REFDIR] [--json]
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
    else:
        args += ["-DCMAKE_BUILD_TYPE=Release"]   # single-config generators ignore --config
    if cmake(*args, logfile=BUILD / "configure.log"):
        sys.exit(f"configure failed: {BUILD / 'configure.log'}")


STEPPED_EXE = "XMen2Stepped.exe" if sys.platform == "win32" else "XMen2Stepped"
STEPPED_GEN_EXE = "XMen2SteppedGen.exe" if sys.platform == "win32" else "XMen2SteppedGen"
NATIVE_EXE = "XMen2Native.exe" if sys.platform == "win32" else "XMen2Native"


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
    """build/handc_src: game/c/*.c, game/include/*.h and platform/*.h as they are now, in one directory, which
    the bridges and the hand-C compile read (C being edited meanwhile can't mix two states into
    one build). Unchanged files keep their timestamps, so only edits recompile."""
    import shutil
    snap = BUILD / "handc_src"
    snap.mkdir(parents=True, exist_ok=True)
    want = {p.name: p for p in list((common.GAME / "c").glob("*.c")) + list((common.GAME / "include").glob("*.h"))
            + list((common.ROOT / "platform").glob("*.h"))}
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
    for mode, d in (("stepped", "bridge"), ("verify", "bridge_verify"), ("native", "bridge_native")):
        r = subprocess.run([sys.executable, str(common.ROOT / "tools" / "bridge.py"), "--out", str(BUILD / d),
                            "--mode", mode, "--src", str(snap)], capture_output=True, text=True)
        if r.returncode:
            sys.exit(f"bridge.py failed: {(r.stdout + r.stderr).strip()[-400:]}")
    # phase 5: XMen2Native's export table, from the same snapshot of the headers
    r = subprocess.run([sys.executable, str(common.ROOT / "tools" / "export_table.py"), "--out", str(BUILD / "native"),
                        "--src", str(snap)], capture_output=True, text=True)
    if r.returncode:
        sys.exit(f"export_table.py failed: {(r.stdout + r.stderr).strip()[-400:]}")
    configure()
    rc = cmake("--build", str(BUILD), "--config", "Release", "--target", "XMen2Recomp", "XMen2Stepped",
               "XMen2SteppedGen", "XMen2Verify", "XMen2Native", "--parallel", logfile=BUILD / "build.log")
    exes = {"exe": exe_path(), "stepped_exe": exe_path(STEPPED_EXE), "stepped_gen_exe": exe_path(STEPPED_GEN_EXE),
            "verify_exe": exe_path(VERIFY_EXE), "native_exe": exe_path(NATIVE_EXE)}
    res = {**{k: str(v) for k, v in exes.items()}, "config": "Release",
           "ok": rc == 0 and all(v.is_file() for v in exes.values()), "log": str(BUILD / "build.log")}
    if a.native:
        # P5 P3: what each game object still needs from the 68000 machine (its COFF symbol table)
        # and which 68000-machine symbols XMen2Native still defines (its link map)
        import native_scan
        build_ok = res["ok"]
        scan = native_scan.scan(BUILD, "XMen2Native", exe_path(NATIVE_EXE))
        # the scan's own "ok" (objects and map found) can't stand for the build's: a failed
        # build over stale objects scans fine
        res = {"exe": res["native_exe"], "log": res["log"], **{k: v for k, v in scan.items() if k != "ok"},
               "build_ok": build_ok, "scan_ok": scan.get("ok"), "ok": build_ok and scan.get("ok") is True}
        return res
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


FBHASH = re.compile(r"^\[FBHASH\] frame=(\d+) w=(\d+) h=(\d+) hash=0x([0-9A-Fa-f]{16})", re.M)


def add_fb_column(sync_csv: Path, stderr: str, logname: Path) -> int:
    """--fb (stepped modes): add `fb_fnv1a64` to every sync row, the picture. In the
    frame-synchronous runner the engine renders the whole frame right after the V-int at each
    sync point (exact_sched.c fsync_render), at the start of the next runner frame, and runner
    frame k ends at sync point k; the runner's --hash-frames 1 prints at the end of every runner
    frame the FNV-1a-64 of its framebuffer's active region (main.c framebuf_active_hash, the raw
    VDP output). So row k gets runner frame k's hash: the frame rendered after sync k-1's V-int
    (row 1: the picture before any, the boot's empty frame)."""
    hashes = [(int(m.group(1)), m.group(4).upper()) for m in FBHASH.finditer(stderr)]
    lines = sync_csv.read_text().splitlines()
    rows = lines[1:]
    if [f for f, _h in hashes] != list(range(1, len(hashes) + 1)) or len(hashes) != len(rows):
        sys.exit(f"--fb: {len(hashes)} [FBHASH] lines (runner frames) for {len(rows)} sync rows; see {logname}")
    sync_csv.write_text("\n".join([lines[0] + ",fb_fnv1a64"] + [r + "," + h for r, (_f, h) in zip(rows, hashes)]) + "\n",
                        newline="\n")
    return len(rows)


STEPPED_MODES = {"stepped": "STEPPED_EXE", "stepped-gen": "STEPPED_GEN_EXE", "handc": "STEPPED_EXE",
                 "native": "NATIVE_EXE"}   # native (P5): the hand-C build whose sync rows are its export's


def cmd_trace(a) -> dict:
    """recomp: the timed runner plays the input (phase 2); --sync adds sync.csv and the
    replay log (phase 4 D1). stepped / stepped-gen: the frame-synchronous untimed runner, with
    hand C / with all generated code, plays the input one sync interval per runner frame (the
    pads of each sync point's reference frame, from REFDIR = the run's D1 trace) and writes
    sync.csv (phase 4 D2: the two must be equal). native: the same with XMen2Native, whose
    sync rows are computed from its exported 68000 image (phase 5); the JSON adds "exported",
    the runner's own count of where the image's bytes came from."""
    import os
    if a.mode not in ("recomp", *STEPPED_MODES):
        sys.exit(f"--mode {a.mode}: not implemented (recomp, {', '.join(STEPPED_MODES)})")
    if a.mode in STEPPED_MODES and not a.replay:
        sys.exit(f"--mode {a.mode} needs --replay REFDIR (the run's `--mode recomp --sync` trace)")
    if (a.options or a.present_log or a.flip) and a.mode != "native":
        sys.exit("--options, --present-log, --flip: only the native runner has them (phase 6)")
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
    copy_runner(src, run_dir)
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
        if a.fb:   # the runner's [FBHASH] of every runner frame (see add_fb_column)
            cmd += ["--hash-frames", "1"]
        if a.options:   # phase 6: the enhancement options (P6-enhance.md "The contract")
            cmd += ["--options", a.options]
        if a.present_log:   # phase 6 E4: OUT/present.csv, one row per displayed frame
            cmd += ["--present-log", str(out / "present.csv")]
        if a.flip:   # phase 6 E4: the latency probe
            cmd += ["--flip", str(a.flip)]
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
           "stub_hits": c["stub_hits"], "fps": b["fps"], "exe": str(src), "exe_sha1": common.sha1(src)}
    if a.mode in STEPPED_MODES:
        res["sync_points"] = csv_rows(sync_csv)
        if a.fb:
            res["fb_rows"] = add_fb_column(sync_csv, stderr, out / "runner.log")
        if a.mode == "native":
            if "exported" not in c:
                sys.exit(f"{src.name} reported no export counts; see {out / 'runner.log'}")
            res["exported"] = c["exported"]
            # phase 6: the effective option values and the enhance counters, the runner's own
            if "options" not in c or "enhance" not in c:
                sys.exit(f"{src.name} reported no options / enhance counters; see {out / 'runner.log'}")
            res["options"], res["enhance"] = c["options"], c["enhance"]
        return drop_runner(a, run_dir, res)
    res.update(frames=csv_rows(out / "frames.csv"), complete=b["complete"])
    if a.sync:
        # stack_low: the fixed bound the sync CRCs mask from (the corpus's lowest A7);
        # reported only if this run never went below it (else the CRCs are not valid).
        a7 = c.get("a7_min")
        low = c.get("stack_low")
        res.update(sync_points=csv_rows(sync_csv), a7_min=a7,
                   stack_low=low if a7 is not None and low is not None and a7 >= low else None)
    return drop_runner(a, run_dir, res)


def copy_runner(src: Path, run_dir: Path) -> None:
    """A private copy of one runner (settings.ini and the evidence files it writes beside itself
    stay per run): the executable and the DLLs next to it, not the build's other runners."""
    for f in src.parent.iterdir():
        if f.is_file() and (f.name == src.name or f.suffix.lower() == ".dll" or f.name == src.stem + ".pdb"):
            shutil.copy2(f, run_dir / f.name)
    (run_dir / "settings.ini").write_text(SETTINGS)


def drop_runner(a, run_dir: Path, res: dict) -> dict:
    """A passed trace deletes its runner copy (~5-30 MB each, x the corpus). It stays for
    debugging when the run failed (the exits above), when a counter is nonzero (its evidence
    files: dispatch_misses.toml...), or with --keep-runner / XMEN2_KEEP_RUNNER=1."""
    import os
    keep = a.keep_runner or os.environ.get("XMEN2_KEEP_RUNNER", "") not in ("", "0")
    if keep or any(res.get(k) for k in ("dispatch_misses", "interp_instructions", "stub_hits")):
        res["runner_dir"] = str(run_dir)
    else:
        shutil.rmtree(run_dir, ignore_errors=True)
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
    hand-written C as shadow tests: game/runtime/verify.cpp) and merge what the runs report.
    Exits non-zero, naming the run and why, when a run fails, plays short, or stopped testing
    early (its JSON's `stopped_early`); the output adds `per_run` (frames, functions_tested,
    calls_checked, calls_refused, seconds)."""
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
    import threading
    import time
    out = Path(a.out).resolve() if a.out else BUILD / "verify"
    out.mkdir(parents=True, exist_ok=True)
    jobs = max(1, min(a.jobs, len(inputs)))
    free: "queue.Queue[Path]" = queue.Queue()
    for k in range(jobs):   # one runner copy per worker: settings.ini and evidence files stay apart
        run_dir = out / f"runner{k}"
        if run_dir.exists():
            shutil.rmtree(run_dir)
        run_dir.mkdir()
        copy_runner(src, run_dir)
        free.put(run_dir)
    # each function's instruction count: verify.cpp probes the calls its sampling skips until
    # the checked calls have covered them all
    insns = out / "insns.txt"
    insns.write_text("".join(f"{addr} {len(pcs)}\n" for addr, pcs in sorted(code_addrs_by_function().items())))

    # A run fails when its runner fails, plays fewer frames than asked, or its testing stopped
    # before the end (verify.cpp `stopped_early`: a fatal error, a C run or synthetic case that
    # never returned, calls refused for frames on end). The first failure kills the runs still
    # going (by their own process) and skips the rest, unless --keep-going.
    stop = threading.Event()
    lock = threading.Lock()
    live: dict = {}            # run -> its runner process
    killed: set[str] = set()   # runs stopped because another one failed

    def fail(run: str, why: str) -> str:
        if not a.keep_going and not stop.is_set():
            stop.set()
            with lock:
                for other, p in live.items():
                    if other != run and p.poll() is None:
                        killed.add(other)
                        p.kill()
        return why

    def one(inp: Path):
        run = inp.stem
        if stop.is_set():
            return run, None, None, None
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
        logf = out / f"{run}.log"
        run_dir = free.get()
        t0 = time.time()
        timed_out = False
        try:
            if stop.is_set():
                return run, None, None, None
            cmd[0] = str(run_dir / src.name)
            log(f"verify {run}: {frames} frames")
            with open(logf, "w", encoding="utf-8", errors="replace") as lf:
                p = subprocess.Popen(cmd, cwd=run_dir, env=env, stdout=subprocess.PIPE, stderr=lf, text=True,
                                     errors="replace")
                with lock:
                    live[run] = p
                try:
                    stdout, _ = p.communicate(timeout=a.timeout)
                except subprocess.TimeoutExpired:
                    p.kill()
                    stdout, _ = p.communicate()
                    timed_out = True
                finally:
                    with lock:
                        live.pop(run, None)
        finally:
            free.put(run_dir)
        secs = round(time.time() - t0, 1)
        if run in killed:
            return run, None, None, secs
        data = json.loads(res.read_text()) if res.exists() else None
        why = (data or {}).get("stopped_early")
        if timed_out:
            return run, data, fail(run, f"verify {run}: timed out after {a.timeout} s; see {logf}"), secs
        if p.returncode != 0 or data is None:
            last = [l for l in logf.read_text(errors="replace").splitlines() if l.startswith(("[handc]", "[verify]"))]
            return run, data, fail(run, f"verify {run}: the verification runner failed ({p.returncode})"
                                        f"{': ' + (why or last[-1]) if why or last else ''}; see {logf}"), secs
        bench = re.search(r"GENESISRECOMP_BENCHMARK (\{.*\})", stdout or "")
        b = json.loads(bench.group(1)) if bench else {}
        if not b.get("complete") or b.get("frames") != frames:
            return run, data, fail(run, f"verify {run}: the runner played {b.get('frames')} of {frames} frames; "
                                        f"see {logf}"), secs
        if why:
            return run, data, fail(run, f"verify {run}: testing stopped early: {why} ({data.get('functions_tested')} "
                                        f"functions tested); see {logf}"), secs
        return run, data, None, secs

    merged: dict[str, dict] = {}
    per_run: dict[str, dict] = {}
    # the longest runs start first (the TAS, then the late fuzz runs: their TAS prefix), so the
    # last ones to finish are short; the merge stays in run order
    longest = sorted(inputs, key=lambda p: -(a.frames or script_frames(p)))
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
        done = {p: f for p, f in zip(longest, [ex.submit(one, p) for p in longest])}
        results = [done[p].result() for p in inputs]
    errs = [err for _run, _data, err, _s in results if err]
    if errs:
        sys.exit("\n".join(errs) + (f"\n({sum(1 for r in results if r[1] is None and not r[2])} other runs stopped or "
                                    f"skipped: rerun with --keep-going for all)" if not a.keep_going else ""))
    if not (a.keep_runner or os.environ.get("XMEN2_KEEP_RUNNER", "") not in ("", "0")):
        for k in range(jobs):
            shutil.rmtree(out / f"runner{k}", ignore_errors=True)
    for run, data, _err, secs in results:   # in run order: the same merge as a serial run
        per_run[run] = {k: data.get(k) for k in ("frames", "functions_tested", "calls_checked", "calls_refused")}
        per_run[run]["seconds"] = secs
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
    return {"runs": len(inputs), "per_run": per_run, "functions": funcs}


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


# --- phase 5: the package (P8 Windows, P9 Linux) ---------------------------------------

PACKAGE_README = """X-Men 2: Clone Wars, native PC port (from the xmen2disasm decompilation).

Run it with your own copy of the game's cartridge (USA/Europe; it checks the SHA-1
61409e6cf6065ab67d8952b891d8edcf47777193 and refuses anything else):

    {exe} path\\to\\xmen2.bin

Nothing of the cartridge is in this package: the program reads the graphics, sound data and
levels from your file each time it starts.

Keyboard (player 1): arrows; Z = A, X = B, C = C, Enter = Start; A = X, S = Y, D = Z, Q = Mode.
Game controllers (players 1 and 2): D-pad or left stick; X = A, A = B, B = C, Start;
LB = X, Y = Y, RB = Z, Back = Mode.  F11: fullscreen.  Esc: quit.

Enhancements, each off unless you turn it on:
    full_rate_2p=1      two players at the full 60 frames a second (the original halves it)
    high_fps=144        frames in between the game's own, for a 120/144/165/240 Hz display
    widescreen=16:9     a 16:9 view in the stages (the Cerebro screens stay 4:3)
on the command line:  {exe} path\\to\\xmen2.bin --options widescreen=16:9,high_fps=144
or in xmen2.ini next to the program:
    [options]
    widescreen=16:9
While playing: F9 turns the in-between frames on and off, F10 the 16:9 view (from the next
screen).  {exe} --options --json lists them.

Licenses of the parts it is built from: LICENSES/.
"""


def cmd_native_build(a) -> dict:
    """Phase 5: XMen2Native alone, with this platform's compiler (P9: the Linux build in WSL): the
    snapshot, the bridges and the export table as `build` makes them, then CMake with
    XMEN2_NATIVE_ONLY (no recompiler, no generated code, no recomp runners) and the one target."""
    common.ensure_rom()
    snap = snapshot_handc()
    for mode, d in (("stepped", "bridge"), ("native", "bridge_native")):
        r = subprocess.run([sys.executable, str(common.ROOT / "tools" / "bridge.py"), "--out", str(BUILD / d),
                            "--mode", mode, "--src", str(snap)], capture_output=True, text=True)
        if r.returncode:
            sys.exit(f"bridge.py failed: {(r.stdout + r.stderr).strip()[-400:]}")
    r = subprocess.run([sys.executable, str(common.ROOT / "tools" / "export_table.py"), "--out", str(BUILD / "native"),
                        "--src", str(snap)], capture_output=True, text=True)
    if r.returncode:
        sys.exit(f"export_table.py failed: {(r.stdout + r.stderr).strip()[-400:]}")
    BUILD.mkdir(parents=True, exist_ok=True)
    if not (BUILD / "CMakeCache.txt").exists():
        args = ["-S", str(common.ROOT), "-B", str(BUILD), "-DXMEN2_NATIVE_ONLY=ON"]
        args += ["-G", GENERATOR, "-A", "x64"] if sys.platform == "win32" else \
                ["-G", "Ninja", "-DCMAKE_BUILD_TYPE=Release"]
        if cmake(*args, logfile=BUILD / "configure.log"):
            sys.exit(f"configure failed: {BUILD / 'configure.log'}")
    rc = cmake("--build", str(BUILD), "--config", "Release", "--target", "XMen2Native", "--parallel",
               logfile=BUILD / "build.log")
    exe = exe_path(NATIVE_EXE)
    return {"ok": rc == 0 and exe.is_file(), "exe": str(exe), "log": str(BUILD / "build.log")}


def package_linux(a) -> dict:
    """P9: the Linux package, built and checked in this machine's WSL Ubuntu (P5-port.md R7):
    `native-build` there (build dir ~/xmen2-native-build/<hash of the tree>), staged here with a README and the
    licenses (SDL2 is the system's: libsdl2-2.0-0), zipped (the runner keeps its exec bit), scanned
    for ROM windows, smoke-tested in WSL (600 frames on the ROM; a changed ROM refused), and its
    TAS trace in WSL as the stepped runners play it (tas_sync: P1's comparison, on Linux)."""
    import zipfile
    sys.path.insert(0, str(common.ROOT / "tools"))
    import harness
    import romscan
    common.ensure_rom()

    def wsl_path(p: Path) -> str:
        s = str(p.resolve()).replace("\\", "/")
        return f"/mnt/{s[0].lower()}{s[2:]}" if re.match(r"^[A-Za-z]:", s) else s

    def wsl(cmd: str, timeout: int = 3600) -> subprocess.CompletedProcess:
        return subprocess.run(["wsl", "--", "bash", "-lc", cmd], capture_output=True, text=True,
                              errors="replace", timeout=timeout)

    import hashlib
    root = wsl_path(common.ROOT)
    bdir = "$HOME/xmen2-native-build/" + hashlib.sha1(root.encode()).hexdigest()[:10]   # one per source tree
    r = wsl(f"cd '{root}' && XMEN2_BUILD={bdir} python3 tools/xmen2check.py native-build --json")
    j = next((json.loads(l) for l in reversed(r.stdout.splitlines()) if l.startswith("{")), None)
    if r.returncode or not j or not j.get("ok"):
        sys.exit(f"WSL native-build failed: {(r.stdout + r.stderr).strip()[-600:]}")
    stage = BUILD / "package" / "xmen2-native-linux"
    if stage.exists():
        shutil.rmtree(stage)
    (stage / "LICENSES").mkdir(parents=True)
    c = wsl(f"cp \"{bdir}/XMen2Native\" '{wsl_path(stage)}/xmen2' && chmod +x '{wsl_path(stage)}/xmen2'")
    if c.returncode:
        sys.exit(f"copying the Linux runner failed: {c.stderr.strip()[-300:]}")
    readme = PACKAGE_README.format(exe="./xmen2").replace("path\\to\\xmen2.bin", "path/to/xmen2.bin")
    readme += "\nLinux: needs SDL2 (Debian/Ubuntu: sudo apt install libsdl2-2.0-0).\n"
    (stage / "README.txt").write_text(readme, encoding="utf-8", newline="\n")
    engine = common.ENGINE / "runner" / "external"
    for src, name in ((common.ENGINE / "LICENSE.md", "segagenesisrecomp-LICENSE.md"),
                      (common.ENGINE / "THIRD-PARTY-LICENSES.md", "segagenesisrecomp-THIRD-PARTY-LICENSES.md"),
                      (engine / "ymfm" / "LICENSE", "ymfm-LICENSE.txt"),
                      (engine / "superzazu" / "LICENSE", "superzazu-z80-LICENSE.txt")):
        shutil.copy2(src, stage / "LICENSES" / name)
    files = sorted(p for p in stage.rglob("*") if p.is_file())
    zpath = BUILD / "package" / "xmen2-native-linux.zip"
    with zipfile.ZipFile(zpath, "w", zipfile.ZIP_DEFLATED) as z:
        for p in files:
            info = zipfile.ZipInfo(p.relative_to(stage).as_posix())
            info.external_attr = ((0o755 if p.name == "xmen2" else 0o644) | 0o100000) << 16
            info.compress_type = zipfile.ZIP_DEFLATED
            z.writestr(info, p.read_bytes())
    rom = common.ROM.read_bytes()
    index = romscan.rom_index(rom)
    windows = {p.relative_to(stage).as_posix(): romscan.scan(p.read_bytes(), rom, index) for p in files}
    nwin = sum(len(v) for v in windows.values())
    # smoke, in WSL
    out = BUILD / "package" / "linux-check"
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True)
    bad = out / "not-xmen2.bin"
    data = bytearray(rom)
    data[0x200] ^= 0xFF
    bad.write_bytes(bytes(data))
    x, rp = f"'{wsl_path(stage)}/xmen2'", f"'{wsl_path(common.ROM)}'"
    s1 = wsl(f"SDL_AUDIODRIVER=dummy {x} {rp} --benchmark 600", timeout=600)
    s2 = wsl(f"SDL_AUDIODRIVER=dummy {x} '{wsl_path(bad)}' --benchmark 60", timeout=600)
    smoke = {"rom": {"exit": s1.returncode, "benchmark": "GENESISRECOMP_BENCHMARK" in s1.stdout},
             "wrong_rom": {"exit": s2.returncode, "refused": "SHA-1" in s2.stderr}}
    # the TAS, as `trace --mode native` plays it
    refs = harness.refs_dir()
    script = out / "input.input"
    frames = stepped_script(refs / "tas.input", refs / "tas-recomp-sync", script)
    sync = out / "sync.csv"
    t = wsl(f"cd '{wsl_path(out)}' && XMEN2_SYNC_CSV='{wsl_path(sync)}' SDL_AUDIODRIVER=dummy {x} {rp} "
            f"--benchmark {frames} --input-script '{wsl_path(script)}'", timeout=7200)
    (out / "runner.log").write_text(t.stderr, encoding="utf-8")
    o = wsl(f"{x} --options --json", timeout=120)
    opts = next((json.loads(l) for l in reversed(o.stdout.splitlines()) if l.startswith("{")), None)
    ok = (nwin == 0 and smoke["rom"]["exit"] == 0 and smoke["rom"]["benchmark"] and smoke["wrong_rom"]["exit"] == 2
          and smoke["wrong_rom"]["refused"] and t.returncode == 0 and sync.is_file())
    return {"ok": ok, "platform": "linux", "zip": str(zpath),
            "files": [p.relative_to(stage).as_posix() for p in files], "rom_windows": nwin,
            "rom_window_hits": {k: v for k, v in windows.items() if v}, "smoke": smoke,
            "tas_sync": str(sync), "tas_frames": frames, "tas_exit": t.returncode, "options": opts}


def runner_options(cmd: list[str], cwd: Path | None = None) -> dict:
    """The runner's own `--options --json` (phase 6)."""
    r = subprocess.run([*cmd, "--options", "--json"], capture_output=True, text=True, timeout=120, cwd=cwd)
    j = next((json.loads(l) for l in reversed(r.stdout.splitlines()) if l.startswith("{")), None)
    if r.returncode or not j or "options" not in j:
        sys.exit(f"{cmd[-1]} --options --json failed ({r.returncode}): {(r.stdout + r.stderr).strip()[-300:]}")
    return j


def cmd_options(a) -> dict:
    """Phase 6 E2 (P6-enhance.md "The contract"): the release runner's options as it lists them,
    and what a private copy of it reports as effective when an xmen2.ini beside it sets every option
    to its first non-default value (the settings file is read)."""
    import tempfile
    src = exe_path(NATIVE_EXE)
    if not src.is_file():
        sys.exit(f"no native runner at {src}: run `xmen2check.py build`")
    listed = runner_options([str(src)])
    want = {k: next(v for v in d["values"] if v != d["default"]) for k, d in listed["options"].items()}
    with tempfile.TemporaryDirectory(prefix="xmen2opt_") as td:
        copy_runner(src, Path(td))
        (Path(td) / "xmen2.ini").write_text("[options]\n" + "".join(f"{k}={v}\n" for k, v in want.items()),
                                            encoding="utf-8")
        got = runner_options([str(Path(td) / src.name)])
    # "options": the option table itself (phase.py 6's E2 reads options[name]); "runner": all the
    # runner printed (package's "options" is that whole report: E6 reads options.options[name])
    return {"ok": got.get("effective") == want and got.get("settings_file_found") is True, "exe": str(src),
            "options": listed["options"], "runner": listed, "set_in_settings": want,
            "from_settings": got.get("effective")}


def cmd_package(a) -> dict:
    """P8 (Windows) / P9 (Linux): the native runner as a package that runs on the user's ROM and
    holds nothing ROM-derived. Builds (the build contract), stages the runner, its SDL2 library, a
    README and the licenses of what it is built from, zips them, scans every packaged file for ROM
    windows (tools/romscan.py), and smoke-tests the unzipped package: 600 headless frames on the ROM
    (exit 0), and refusal of a ROM with one byte changed (the SHA-1 check, exit 2)."""
    import os
    import tempfile
    import zipfile
    sys.path.insert(0, str(common.ROOT / "tools"))
    import romscan
    if a.platform == "linux":
        return package_linux(a)
    common.ensure_rom()
    b = cmd_build(argparse.Namespace(native=False, handc=False, json=True))
    exe = Path(b["native_exe"])
    if not b["ok"]:   # never zip a stale runner over a failed build
        sys.exit(f"the build failed, see {b['log']}")
    if not exe.is_file():
        sys.exit(f"no native runner at {exe}")
    engine = common.ENGINE / "runner" / "external"
    stage = BUILD / "package" / "xmen2-native-windows"
    if stage.exists():
        shutil.rmtree(stage)
    (stage / "LICENSES").mkdir(parents=True)
    shutil.copy2(exe, stage / "XMen2.exe")
    shutil.copy2(exe.parent / "SDL2.dll", stage / "SDL2.dll")
    (stage / "README.txt").write_text(PACKAGE_README.format(exe="XMen2.exe"), encoding="utf-8", newline="\r\n")
    for src, name in ((common.ENGINE / "LICENSE.md", "segagenesisrecomp-LICENSE.md"),
                      (common.ENGINE / "THIRD-PARTY-LICENSES.md", "segagenesisrecomp-THIRD-PARTY-LICENSES.md"),
                      (engine / "SDL2" / "SDL2-2.28.5" / "COPYING.txt", "SDL2-COPYING.txt"),
                      (engine / "ymfm" / "LICENSE", "ymfm-LICENSE.txt"),
                      (engine / "superzazu" / "LICENSE", "superzazu-z80-LICENSE.txt")):
        shutil.copy2(src, stage / "LICENSES" / name)
    files = sorted(p for p in stage.rglob("*") if p.is_file())
    zpath = BUILD / "package" / "xmen2-native-windows.zip"
    with zipfile.ZipFile(zpath, "w", zipfile.ZIP_DEFLATED) as z:
        for p in files:
            z.write(p, p.relative_to(stage).as_posix())
    rom = common.ROM.read_bytes()
    index = romscan.rom_index(rom)
    windows = {p.relative_to(stage).as_posix(): romscan.scan(p.read_bytes(), rom, index) for p in files}
    nwin = sum(len(v) for v in windows.values())
    smoke = {}
    with tempfile.TemporaryDirectory(prefix="xmen2pkg_") as td:
        with zipfile.ZipFile(zpath) as z:
            z.extractall(td)
        run = Path(td) / "XMen2.exe"
        env = {**os.environ, "SDL_AUDIODRIVER": "dummy"}
        r = subprocess.run([str(run), str(common.ROM), "--benchmark", "600"], capture_output=True, text=True,
                           timeout=600, cwd=td, env=env)
        smoke["rom"] = {"exit": r.returncode, "benchmark": "GENESISRECOMP_BENCHMARK" in r.stdout}
        bad = Path(td) / "not-xmen2.bin"
        data = bytearray(rom)
        data[0x200] ^= 0xFF
        bad.write_bytes(bytes(data))
        r = subprocess.run([str(run), str(bad), "--benchmark", "60"], capture_output=True, text=True,
                           timeout=600, cwd=td, env=env)
        smoke["wrong_rom"] = {"exit": r.returncode, "refused": "SHA-1" in r.stderr}
    ok = (nwin == 0 and smoke["rom"]["exit"] == 0 and smoke["rom"]["benchmark"]
          and smoke["wrong_rom"]["exit"] == 2 and smoke["wrong_rom"]["refused"])
    return {"ok": ok, "platform": "windows", "zip": str(zpath),
            "files": [p.relative_to(stage).as_posix() for p in files], "rom_windows": nwin,
            "rom_window_hits": {k: v for k, v in windows.items() if v}, "smoke": smoke,
            "options": runner_options([str(stage / "XMen2.exe")])}


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
    p.add_argument("--out", default="", help="output directory (default build/verify)")
    p.add_argument("--keep-going", action="store_true", help="play every run even after one failed")
    p.add_argument("--keep-runner", action="store_true", help="keep the runner copies after a pass (XMEN2_KEEP_RUNNER=1)")
    for name in ("gen", "build", "trace"):
        p = sub.add_parser(name)
        p.add_argument("--json", action="store_true")
        if name == "build":
            p.add_argument("--handc", action="store_true", help="P4 D6: count generated game functions linked")
            p.add_argument("--native", action="store_true",
                           help="P5: what XMen2Native's objects still need from the 68000 machine, and its link map")
        if name == "trace":
            p.add_argument("--run", required=True)
            p.add_argument("--input", default="")
            p.add_argument("--out", required=True)
            p.add_argument("--frames", type=int, default=0)
            p.add_argument("--mode", default="recomp")
            p.add_argument("--sync", action="store_true", help="recomp: also sync.csv + replay.bin (P4 D1)")
            p.add_argument("--replay", default="", help="stepped: the D1 trace dir to replay")
            p.add_argument("--fb", action="store_true",
                           help="stepped modes: + sync.csv column fb_fnv1a64, the frame rendered at the last sync point")
            p.add_argument("--timeout", type=int, default=7200)
            p.add_argument("--dump-frames", default="", help="dev: dump full state after these frames")
            p.add_argument("--keep-runner", action="store_true",
                           help="keep OUT/runner (the private runner copy) after a pass (XMEN2_KEEP_RUNNER=1)")
            p.add_argument("--options", default="", help="native (phase 6): the runner's --options K=V,...")
            p.add_argument("--present-log", action="store_true",
                           help="native (phase 6 E4): OUT/present.csv, one row per displayed frame")
            p.add_argument("--flip", type=int, default=0,
                           help="native (phase 6 E4): player 1's C inverted in runner frames N to N+7")
    p = sub.add_parser("options")
    p.add_argument("--json", action="store_true")
    p = sub.add_parser("package")
    p.add_argument("--json", action="store_true")
    p.add_argument("--platform", default="windows", choices=["windows", "linux"])
    p = sub.add_parser("native-build")
    p.add_argument("--json", action="store_true")
    a = ap.parse_args()
    res = {"gen": cmd_gen, "build": cmd_build, "trace": cmd_trace, "types": cmd_types,
           "handc": cmd_handc, "lint": cmd_lint, "verify": cmd_verify, "package": cmd_package,
           "native-build": cmd_native_build, "options": cmd_options}[a.cmd](a)
    if a.cmd == "gen" and not a.json:
        res = {k: v for k, v in res.items() if k != "functions"}
    print(json.dumps(res, indent=None if a.json else 1))
    # a failed build is a failed command (a link over a running runner exited 0 and left the old
    # exe: 2026-10-21); the other commands' "ok" is a result their callers grade
    return 1 if a.cmd in ("build", "native-build") and res.get("ok") is False else 0


if __name__ == "__main__":
    raise SystemExit(main())
