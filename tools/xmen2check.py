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
    return write_script(stepped_pads(inp, ref), out)


def stepped_pads(inp: Path, ref: Path) -> list[str]:
    """stepped_script's pads, one per runner frame (index 0 = runner frame 1)."""
    pads, frames = input_pads(inp), sync_frames(ref)
    return [pads[0] if pads else "0 0"] + [pads[min(f, len(pads)) - 1] if pads else "0 0" for f in frames[:-1]]


def write_script(seq: list[str], out: Path) -> int:
    """An input script of one runner frame per pads entry; returns the frames."""
    lines, prev, n = [], None, 0
    for p in seq:
        if p != prev and prev is not None:
            lines += [f"PADS {prev}", f"WAIT {n}"]
            n = 0
        prev, n = p, n + 1
    if prev is not None:
        lines += [f"PADS {prev}", f"WAIT {n}"]
    out.write_text("# per sync interval (xmen2check.py trace --mode stepped)\n" + "\n".join(lines) + "\n", newline="\n")
    return len(seq)


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


def picture_free(a) -> bool:
    """A stepped trace that nothing compares a picture in: the runner gets --no-render, and its sync
    points render no frame (exact_sched.c fsync_render, machine.cpp plat_sync_point; ~2/3 of such a
    trace's time). The render changes nothing the game reads there, so the rows stay the same (the
    savings and the proof: NOTES.md "Phase 5 harness"). Everything that looks at the picture or the
    platform's per-frame work keeps it: --fb, phase 6's options, present log, flips and game rows,
    phase 7's sessions, scenes, digests and probes, XMEN2_SHOTS / XMEN2_CAPTURE."""
    import os
    return not (a.render or a.fb or a.options or a.present_log or a.flip or a.flip_scan or a.game_rows
                or a.session is not None or a.scene is not None or a.digest or a.state_probe or a.state_skip
                or os.environ.get("XMEN2_SHOTS") or os.environ.get("XMEN2_CAPTURE"))


def crash_note(log_text: str) -> str:
    """A crashed runner's own report (game/runtime/crash_win.cpp: the exception, where, the host
    stack named from the linker map), for a failure message: '' when there is none."""
    lines = [l.strip() for l in log_text.splitlines() if l.startswith("[crash]")]
    return ("; " + " | ".join(lines[:6])) if lines else ""


def crash_env(src: Path) -> dict:
    """The runner's map for its crash report: runner copies don't carry it."""
    m = src.with_suffix(".map")
    return {"XMEN2_CRASH_MAP": str(m)} if m.is_file() else {}


def stepped_args(a, out: Path, hash_out: Path | None = None) -> list[str]:
    """A stepped runner's options for a trace's flags, its files in OUT (hash_out: the [FBHASH]
    lines to that file, not stderr: a branch plan's runs)."""
    cmd = []
    if a.fb:   # the runner's [FBHASH] of every runner frame (see add_fb_column)
        cmd += ["--hash-frames", "1"] + (["--hash-out", str(hash_out)] if hash_out else [])
    if picture_free(a):
        cmd += ["--no-render"]
    if a.options:   # phase 6: the enhancement options (P6-enhance.md "The contract")
        cmd += ["--options", a.options]
    if a.present_log:   # phase 6 E4: OUT/present.csv, one row per displayed frame
        cmd += ["--present-log", str(out / "present.csv")]
    if getattr(a, "flip", 0):   # phase 6 E4: the latency probe
        cmd += ["--flip", str(a.flip)]
    if getattr(a, "flip_scan", ""):   # phase 6 E4: every probe of a span in one run, OUT/flip_scan.csv
        cmd += ["--flip-scan", a.flip_scan, "--probe-out", str(out)] + (["--flip-scan-first"] if a.flip_scan_first else [])
    if a.game_rows:   # phase 6 R8: OUT/game.csv, the game's state less the picture's
        cmd += ["--game-rows", str(out / "game.csv")]
    # phase 7 (P7-multiplayer.md "The contract")
    if a.session is not None:
        cmd += ["--session", str(a.session)]
    if a.scene is not None:
        cmd += ["--scene", str(a.scene)]
    if a.digest:
        cmd += ["--digest", str(out / "digest.csv")]
    if a.state_probe:
        cmd += ["--state-probe", a.state_probe, "--probe-out", str(out)]
    if a.state_skip:
        cmd += ["--state-skip", a.state_skip]
    return cmd


def stepped_result(a, src: Path, out: Path, b: dict, c: dict, fb_text: str) -> dict:
    """A stepped trace's JSON from its runner's benchmark line (b), counters (c) and [FBHASH] lines
    (fb_text), its files in OUT (sync.csv gets the fb column with --fb)."""
    res = {"dispatch_misses": c["dispatch_misses"], "interp_instructions": c["interp_instructions"],
           "stub_hits": c["stub_hits"], "fps": b["fps"], "exe": str(src), "exe_sha1": common.sha1(src)}
    res["sync_points"] = csv_rows(out / "sync.csv")
    res["render"] = not picture_free(a)
    if a.fb:
        res["fb_rows"] = add_fb_column(out / "sync.csv", fb_text, out / "runner.log")
    if a.mode == "native":
        if "exported" not in c:
            sys.exit(f"{src.name} reported no export counts; see {out / 'runner.log'}")
        res["exported"] = c["exported"]
        # phase 6: the effective option values and the enhance counters, the runner's own
        if "options" not in c or "enhance" not in c:
            sys.exit(f"{src.name} reported no options / enhance counters; see {out / 'runner.log'}")
        res["options"], res["enhance"] = c["options"], c["enhance"]
        # phase 7: the savestate's sections, the session machine, the multiplayer counters
        for k in ("state", "session", "mp"):
            if k in c:
                res[k] = c[k]
    return res


def cmd_trace(a) -> dict:
    """recomp: the timed runner plays the input (phase 2); --sync adds sync.csv and the
    replay log (phase 4 D1). stepped / stepped-gen: the frame-synchronous untimed runner, with
    hand C / with all generated code, plays the input one sync interval per runner frame (the
    pads of each sync point's reference frame, from REFDIR = the run's D1 trace) and writes
    sync.csv (phase 4 D2: the two must be equal). native: the same with XMen2Native, whose
    sync rows are computed from its exported 68000 image (phase 5); the JSON adds "exported",
    the runner's own count of where the image's bytes came from.
    Phase 7 (P7-multiplayer.md "The contract"): native without --replay plays --input by runner
    frame, as a played run and a netplay seat do; --session SEED the session machine, --scene N a
    new game at scene N, --digest OUT/digest.csv, --state-probe P:L OUT/probe.csv and
    OUT/probe_snap.csv (--state-skip NAME: the negative control); the JSON adds "state" and
    "session"."""
    import os
    if a.mode not in ("recomp", *STEPPED_MODES):
        sys.exit(f"--mode {a.mode}: not implemented (recomp, {', '.join(STEPPED_MODES)})")
    if a.mode in STEPPED_MODES and a.mode != "native" and not a.replay:
        sys.exit(f"--mode {a.mode} needs --replay REFDIR (the run's `--mode recomp --sync` trace)")
    if (a.options or a.present_log or a.flip or a.flip_scan or a.game_rows) and a.mode != "native":
        sys.exit("--options, --present-log, --flip, --flip-scan, --game-rows: only the native runner has them (phase 6)")
    if a.flip_scan and not a.present_log:
        sys.exit("--flip-scan needs --present-log")
    if a.flip_scan_first and not a.flip_scan:
        sys.exit("--flip-scan-first needs --flip-scan")
    p7 = a.session is not None or a.scene is not None or a.digest or a.state_probe or a.state_skip
    if p7 and a.mode != "native":
        sys.exit("--session, --scene, --digest, --state-probe, --state-skip: only the native runner has them (phase 7)")
    if a.state_skip and not a.state_probe:
        sys.exit("--state-skip needs --state-probe")
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
    env = {**os.environ, "GENESIS_NO_LAUNCHER": "1", "SDL_AUDIODRIVER": "dummy", **crash_env(src)}
    sync_csv = out / "sync.csv"
    if a.mode in STEPPED_MODES:
        if not a.input:
            sys.exit(f"--mode {a.mode} needs --input (the run's exported .input)")
        script = out / "input.input"
        if a.replay:
            ref = Path(a.replay).resolve()
            if not (ref / "sync.csv").is_file():
                sys.exit(f"{ref} has no sync.csv (a `trace --mode recomp --sync` output)")
            frames = stepped_script(Path(a.input), ref, script)
        else:   # native, phase 7: the input by runner frame (a played run's, a netplay seat's)
            frames = script_frames(Path(a.input))
            script.write_text("".join(l for l in Path(a.input).read_text().splitlines(True)
                                      if not l.strip().upper().startswith("EXIT")), newline="\n")
        if a.frames:
            frames = min(frames, a.frames)
        env.update(XMEN2_SYNC_CSV=str(sync_csv))
        env.pop("GENESIS_REPLAY_IN", None)
        cmd = [str(run_dir / src.name), str(common.ROM), "--benchmark", str(frames),
               "--input-script", str(script), "--fresh-evidence", *stepped_args(a, out)]
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
        sys.exit(f"runner failed ({r.returncode}){crash_note(stderr)}; see {out / 'runner.log'}")
    b, c = json.loads(bench.group(1)), json.loads(counters.group(1))
    if a.mode in STEPPED_MODES:
        return drop_runner(a, run_dir, stepped_result(a, src, out, b, c, stderr))
    res = {"dispatch_misses": c["dispatch_misses"], "interp_instructions": c["interp_instructions"],
           "stub_hits": c["stub_hits"], "fps": b["fps"], "exe": str(src), "exe_sha1": common.sha1(src)}
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
    fixed_base(run_dir / src.name)
    (run_dir / "settings.ini").write_text(SETTINGS)


def fixed_base(exe: Path) -> None:
    """A runner copy that Windows loads at its preferred base: the PE header's DYNAMICBASE flag
    cleared (no ASLR for this copy). A state file holds the game fiber's raw stack, return addresses
    included, so it loads only into a process with the executable where the saving one had it
    (game/runtime/state_place.cpp; the build's own runners keep ASLR)."""
    if sys.platform != "win32":
        return
    b = bytearray(exe.read_bytes())
    pe = int.from_bytes(b[0x3C:0x40], "little")
    at = pe + 24 + 70   # IMAGE_OPTIONAL_HEADER64.DllCharacteristics
    if b[pe:pe + 4] != b"PE\0\0" or int.from_bytes(b[pe + 24:pe + 26], "little") != 0x20B:
        sys.exit(f"{exe}: not a 64-bit PE image")
    ch = int.from_bytes(b[at:at + 2], "little")
    if ch & 0x40:
        b[at:at + 2] = (ch & ~0x40).to_bytes(2, "little")
        exe.write_bytes(bytes(b))


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


def cmd_trace_group(a) -> dict:
    """Native traces of many corpus runs at once, each as `trace --mode native --run R --input I
    --replay D --out O` with this command's flags would write it (O's files, the per-run JSON in
    the result), without replaying the TAS: a run whose runner-frame pads start with the TAS's for
    BRANCH_MIN frames or more branches from the TAS at the last frame they share, not from power-on.
    By default each such run is a process of its own that starts from a state file the TAS run saved
    there (the runner's --state-at / --state-load: state_units); --in-process: a branch of a process
    playing the TAS (--branch-plan: from a save in the same process), grouped by pack_branches.
    --runs FILE: a JSON list of {"run", "input", "replay", "out"}; the TAS's own entry (run "tas") is
    the producer (its carrier), else --tas-input/--tas-replay give it. --no-branch: every run alone
    (the control). Prints {run: result or {"error": why}}; exits 1 when any failed."""
    import concurrent.futures
    import os
    import tempfile
    if a.mode != "native":
        sys.exit("trace-group: --mode native only (the runner with --branch-plan)")
    a.jobs = a.jobs or max(1, (os.cpu_count() or 2) - 2)
    common.ensure_rom()
    src = exe_path(NATIVE_EXE)
    if not src.is_file():
        sys.exit(f"no Release runner at {src}: run `xmen2check.py build`")
    entries = json.loads(Path(a.runs).read_text())
    runs = {e["run"]: e for e in entries}
    if len(runs) != len(entries):
        sys.exit("trace-group: a run is listed twice")
    pads, frames = {}, {}
    for r, e in runs.items():
        out = Path(e["out"]).resolve()
        out.mkdir(parents=True, exist_ok=True)
        ref = Path(e["replay"]).resolve()
        if not (ref / "sync.csv").is_file():
            sys.exit(f"{ref} has no sync.csv (a `trace --mode recomp --sync` output)")
        pads[r] = stepped_pads(Path(e["input"]), ref)
        frames[r] = write_script(pads[r], out / "input.input")
    tas_pads = pads.get("tas")
    if tas_pads is None and a.tas_input and a.tas_replay:
        tas_pads = stepped_pads(Path(a.tas_input), Path(a.tas_replay).resolve())
    work = Path(tempfile.mkdtemp(prefix="trace-group-", dir=common.BUILD))
    carrier_script = None
    if tas_pads is not None:
        carrier_script = work / "tas.input"
        write_script(tas_pads, carrier_script)
    branches, plain = [], []
    for r in runs:
        n = 0
        if tas_pads is not None and r != "tas" and not a.no_branch:
            lim = min(len(pads[r]), len(tas_pads), frames[r])
            while n < lim and pads[r][n] == tas_pads[n]:
                n += 1
        if n >= BRANCH_MIN and n < frames[r]:
            branches.append((n, frames[r] - n, r, None))
        elif r != "tas":
            plain.append((None, frames[r], r))
    own = (None, frames["tas"], "tas") if "tas" in runs else None
    if a.in_process:
        units = pack_branches(branches, own, plain, None, max(frames.values(), default=0), a.jobs)
    else:
        units = state_units(branches, own, plain, carrier_script)
    states = StateFiles(work / "states") if any(u.get("states") for u in units) else None
    gate = common.MemGate()
    results: dict[str, dict] = {}
    done: list[tuple] = []   # each process's outputs: read when every process has ended (a state's file copies)

    def one(u: dict) -> None:
        if "load" in u and not states.wait(u["load"], None):   # a branch: its state first, outside the gate
            results[u["run"]] = {"error": f"the TAS run ended before saving frame {u['load']}; see "
                                          f"{(Path(runs['tas']['out']).resolve() if own else work / units[0]['name']) / 'runner.log'}"}
            return
        with gate:
            play(u)

    def play(u: dict) -> None:
        mine = u["run"]
        d = Path(runs[mine]["out"]).resolve() if mine else work / u["name"]
        d.mkdir(parents=True, exist_ok=True)
        script = d / "input.input" if mine else carrier_script
        run_dir = d / "runner"
        if run_dir.exists():
            shutil.rmtree(run_dir)
        run_dir.mkdir()
        copy_runner(src, run_dir)
        env = {**os.environ, "GENESIS_NO_LAUNCHER": "1", "SDL_AUDIODRIVER": "dummy", "XMEN2_SYNC_CSV": str(d / "sync.csv"),
               **crash_env(src)}
        env.pop("GENESIS_REPLAY_IN", None)
        hash_out = bool(u["branches"]) or states is not None   # the [FBHASH] lines in a file: branches copy it
        cmd = [str(run_dir / src.name), str(common.ROM), "--benchmark", str(u["frames"]), "--input-script", str(script),
               "--fresh-evidence", *stepped_args(a, d, d / "fbhash.log" if hash_out else None)]
        if states:   # state files: every process of the group with one /GS cookie
            env["XMEN2_GS_COOKIE"] = states.cookie
        if u.get("states"):   # the runner's --state-at: FRAME PATH
            cmd += ["--state-at", str(states.plan(u["states"], d / "states.txt"))]
        if "load" in u:
            cmd += ["--state-load", str(states.path(u["load"]))]
        if u["branches"]:
            plan = d / "branches.txt"
            plan.write_text("".join(f"{at} {at + n} {Path(runs[r]['out']).resolve()} "
                                    f"{Path(runs[r]['out']).resolve() / 'input.input'}\n"
                                    for at, n, r, _p in u["branches"]), newline="\n")
            cmd += ["--branch-plan", str(plan)]
        log(f"trace-group {u['name']}: {u['frames']} frames" + (f" + {len(u['branches'])} branches" if u["branches"] else "")
            + (f", {len(u['states'])} states" if u.get("states") else "") + (f" from frame {u['load']}" if "load" in u else ""))
        with open(d / "runner.log", "w", encoding="utf-8", errors="replace") as lf:
            try:
                r = subprocess.run(cmd, cwd=run_dir, env=env, stdout=subprocess.PIPE, stderr=lf, text=True,
                                   errors="replace", timeout=a.timeout)
                code, stdout = r.returncode, r.stdout
            except subprocess.TimeoutExpired:
                code, stdout = -1, ""
            finally:
                if u.get("states"):
                    states.producer_done.set()
        if mine:
            (d / "runner.stdout").write_text(stdout, encoding="utf-8")
        done.append((u, code, stdout, d, run_dir))

    def finish(u: dict, code: int, stdout: str, d: Path, run_dir: Path) -> None:
        mine = u["run"]
        stderr = (d / "runner.log").read_text(errors="replace")
        hash_out = bool(u["branches"]) or states is not None
        outs = ([(mine, d, stdout, stderr, stderr if not hash_out else
                  (d / "fbhash.log").read_text(errors="replace") if (d / "fbhash.log").exists() else "")] if mine else [])
        for _at, _n, r, _p in u["branches"]:
            o = Path(runs[r]["out"]).resolve()
            rd = lambda f: (o / f).read_text(errors="replace") if (o / f).exists() else ""   # noqa: E731
            outs.append((r, o, rd("runner.stdout"), rd("runner.log"), rd("fbhash.log")))
        for r, o, so, se, fb in outs:
            bench = re.search(r"GENESISRECOMP_BENCHMARK (\{.*\})", so)
            counters = re.search(r"\[xmen2\] counters (\{.*\})", se)
            if code != 0 or not bench or not counters:
                results[r] = {"error": f"runner failed ({code}) in {u['name']}{crash_note(stderr)}; "
                                       f"see {d / 'runner.log'}"}
                continue
            try:
                res = stepped_result(a, src, o, json.loads(bench.group(1)), json.loads(counters.group(1)), fb)
            except SystemExit as e:
                results[r] = {"error": str(e)}
                continue
            res["process"] = u["name"]
            results[r] = res
            (o / "trace.json").write_text(json.dumps(res, indent=1), encoding="utf-8")
        keep = a.keep_runner or os.environ.get("XMEN2_KEEP_RUNNER", "") not in ("", "0")
        if code == 0 and not keep:
            shutil.rmtree(run_dir, ignore_errors=True)

    order = units if states else sorted(units, key=lambda u: -u["cost"])   # state_units: already in order
    with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, min(a.jobs, len(units)))) as ex:
        list(ex.map(one, order))
    for args in done:   # sync.csv gets its fb column only now: a branch's process copies the TAS run's rows
        finish(*args)
    shutil.rmtree(work, ignore_errors=True)
    failed = sorted(r for r in runs if "error" in results.get(r, {"error": "not played"}))
    out = {"runs": len(runs), "processes": [{"name": u["name"], "frames": u["frames"],
                                             "branches": [b[2] for b in u["branches"]],
                                             **({"states": u["states"]} if u.get("states") else {}),
                                             **({"loaded_at": u["load"]} if "load" in u else {})} for u in units],
           "failed": failed, "per_run": {r: results.get(r, {"error": "not played"}) for r in runs}}
    if failed:
        print(json.dumps(out))
        sys.exit(1)
    return out


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


BRANCH_MIN = 600   # frames of the TAS's input a run must start with to branch from a TAS run


def verify_units(inputs: list[Path], tas: Path | None, cap: int, jobs: int, states: bool = True) -> list[dict]:
    """The runner processes a verify plays: {"input", "frames", "run" (the run whose results the
    process's own JSON are, or None), "branches": [(at, frames, run, input)]}, and in states mode
    "states" (the frames a process saves states at) or "load" (the frame it starts from).

    A run whose input starts with the TAS's first `at` >= BRANCH_MIN frames (the fuzz runs, most
    scenarios) branches from the TAS at `at`, not from power-on: same results (verify.cpp's testing
    state goes with the machine's), without replaying the prefix. states (the default): a process
    playing the TAS saves a state file at every such frame (the runner's --state-at) and each branch
    is a process of its own from its state (--state-load), started as soon as the file is there
    (state_units). Else in-process (the runner's --branch-plan: snapshot at `at`, the run's remaining
    frames, back to the snapshot): a process playing the TAS for branches stops at its last one,
    unless it is the TAS run itself; branches latest first, each into the TAS run or the last process
    opened if that keeps it within a bound T, else into a new process; T is the one (from the longest
    run's frames up) whose processes, longest first on `jobs` workers, finish first (pack_branches)."""
    want = {p: min(cap, script_frames(p)) if cap else script_frames(p) for p in inputs}
    branches, plain = [], []
    tas_pads = input_pads(tas) if tas else []
    for p in inputs:
        if tas is None or p.stem == "tas":
            plain.append(p)
            continue
        pads, n = input_pads(p), 0
        lim = min(len(pads), len(tas_pads), want[p])
        while n < lim and pads[n] == tas_pads[n]:
            n += 1
        if n >= BRANCH_MIN and n < want[p]:
            branches.append((n, want[p] - n, p.stem, p))
        else:
            plain.append(p)
    tas_run = next((p for p in plain if p.stem == "tas"), None)
    plain = [p for p in plain if p is not tas_run]
    own = (tas_run, want[tas_run], "tas") if tas_run else None
    if states:
        return state_units(branches, own, [(p, want[p], p.stem) for p in plain], tas)
    return pack_branches(branches, own, [(p, want[p], p.stem) for p in plain], tas,
                         max([want[p] for p in inputs], default=0), jobs)


def state_units(branches: list[tuple], own: tuple | None, plain: list[tuple], carrier) -> list[dict]:
    """verify_units' (and trace-group's) processes in states mode: the TAS run `own` (input, frames,
    run), or with no TAS run a carrier playing `carrier`, the TAS's input, up to the last branch
    frame, saves a state at every frame a branch (at, frames after it, run, input) starts from; each
    branch is a process of its own from its state ("load"); `plain` runs play alone. Order: the
    producer, the plain runs longest first, the branches by frame (each waits for its state)."""
    ats = sorted({b[0] for b in branches})
    units = []
    if own or ats:
        units.append({"input": own[0] if own else carrier, "frames": own[1] if own else ats[-1],
                      "run": own[2] if own else None, "branches": [], "states": ats,
                      "name": own[2] if own else f"tas-to-{ats[-1]}"})
    units += sorted(({"input": p, "frames": n, "run": r, "branches": [], "name": r} for p, n, r in plain),
                    key=lambda u: -u["frames"])
    units += [{"input": p, "frames": at + n, "run": r, "branches": [], "load": at, "name": r}
              for at, n, r, p in sorted(branches, key=lambda b: (b[0], b[2]))]
    for u in units:
        u["cost"] = u["frames"] - u.get("load", 0)
    return units


class StateFiles:
    """The state files of a states-mode group (state_units): one per branch frame, under `root`; a
    branch's process waits for its file (or the producer's end, or a stop) before it starts. One
    /GS cookie for every process of the group (XMEN2_GS_COOKIE: game/runtime/state_place.cpp)."""

    def __init__(self, root: Path):
        import secrets
        import threading
        self.root = root
        self.cookie = secrets.token_hex(6)
        self.producer_done = threading.Event()
        if root.exists():
            shutil.rmtree(root)
        root.mkdir(parents=True)

    def path(self, at: int) -> Path:
        return self.root / f"{at}.state"

    def plan(self, ats: list[int], out: Path) -> Path:
        out.write_text("".join(f"{at} {self.path(at)}\n" for at in ats), newline="\n")
        return out

    def wait(self, at: int, stop) -> Path | None:
        """The state of frame `at` once written; None when the producer ended without it or `stop` is set."""
        import time
        p = self.path(at)
        while not p.exists():
            if (stop is not None and stop.is_set()) or self.producer_done.is_set():
                return p if p.exists() else None
            time.sleep(0.2)
        return p

    def done(self) -> None:
        shutil.rmtree(self.root, ignore_errors=True)


def pack_branches(branches: list[tuple], own: tuple | None, plain: list[tuple], carrier, longest: int,
                  jobs: int) -> list[dict]:
    """verify_units' (and trace-group's) processes: branches (at, frames after it, run, input) go
    latest first into the TAS run `own` (input, frames, run) or the last carrier process opened (one
    playing `carrier`, the TAS's input, up to its first branch) while that keeps it within a bound T,
    else into a new carrier; `plain` (input, frames, run) runs play alone. T is the one (from the
    longest run's frames up) whose processes, longest first on `jobs` workers, finish first."""
    import heapq
    branches = sorted(branches, key=lambda b: (-b[0], b[2]))

    def group(bound: int) -> tuple[list[dict], int]:
        units = []
        mine = {"input": own[0], "frames": own[1], "run": own[2], "branches": []} if own else None
        cost = own[1] if own else 0
        cur = None
        for b in branches:
            if mine is not None and cost + b[1] <= bound:
                mine["branches"].append(b)
                cost += b[1]
            elif cur is not None and cur["cost"] + b[1] <= bound:
                cur["branches"].append(b)
                cur["cost"] += b[1]
            else:
                cur = {"input": carrier, "frames": b[0], "run": None, "branches": [b], "cost": b[0] + b[1]}
                units.append(cur)
        units = ([mine] if mine else []) + units + [{"input": p, "frames": n, "run": r, "branches": []}
                                                    for p, n, r in plain]
        for k, u in enumerate(units):
            u["branches"].sort(key=lambda b: (b[0], b[2]))
            u["cost"] = u["frames"] + sum(b[1] for b in u["branches"])
            u["name"] = u["run"] or f"tas{k}-to-{u['frames']}"
        h = [0] * max(1, jobs)
        for c in sorted((u["cost"] for u in units), reverse=True):
            heapq.heappush(h, heapq.heappop(h) + c)
        return units, max(h)

    best = None
    for k in range(0, 41):
        units, span = group(longest + longest * k // 20)
        if best is None or span < best[1]:
            best = (units, span)
    return best[0] if best else []


def cmd_verify(a) -> dict:
    """Play every corpus input on the verification runner (all generated code, timed, with the
    hand-written C as shadow tests: game/runtime/verify.cpp) and merge what the runs report.
    Exits non-zero, naming the run and why, when a run fails, plays short, or stopped testing
    early (its JSON's `stopped_early`); the output adds `per_run` (frames, functions_tested,
    calls_checked, calls_refused, seconds) and `processes` (how the runs were played: runs that
    start with the TAS's input branch from a TAS run, from its state files or, --in-process, as its
    branches (verify_units); --no-branch: each from power-on, the control)."""
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
    tas = Path(a.inputs) / "tas.input"
    units = verify_units(inputs, None if a.no_branch or not tas.is_file() else tas, a.frames, a.jobs,
                         states=not a.in_process)
    states = StateFiles(out / "states") if any(u.get("states") for u in units) else None
    jobs = max(1, min(a.jobs, len(units)))
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
    # never returned, calls refused for frames on end). The first failure kills the processes
    # still going (by their own process) and skips the rest, unless --keep-going.
    stop = threading.Event()
    lock = threading.Lock()
    live: dict = {}            # process name -> its runner process
    killed: set[str] = set()   # processes stopped because another one failed

    def fail(name: str, why: str) -> str:
        if not a.keep_going and not stop.is_set():
            stop.set()
            with lock:
                for other, p in live.items():
                    if other != name and p.poll() is None:
                        killed.add(other)
                        p.kill()
        return why

    def script_of(inp: Path) -> Path:
        script = out / f"{inp.stem}.input"
        script.write_text("".join(l for l in inp.read_text().splitlines(True)
                                  if not l.strip().upper().startswith("EXIT")), newline="\n")
        return script

    def check(run: str, data, logf: Path, played, frames: int, secs) -> tuple:
        """One run's verdict: (run, data, error or None, seconds)."""
        why = (data or {}).get("stopped_early")
        if data is None:
            return run, data, fail(run, f"verify {run}: no results; see {logf}"), secs
        if not played or not played.get("complete") or played.get("frames") != frames:
            return run, data, fail(run, f"verify {run}: the runner played {(played or {}).get('frames')} of {frames} "
                                        f"frames; see {logf}"), secs
        if why:
            return run, data, fail(run, f"verify {run}: testing stopped early: {why} ({data.get('functions_tested')} "
                                        f"functions tested); see {logf}"), secs
        return run, data, None, secs

    def one(u: dict) -> list[tuple]:
        """One runner process: its own run (if any) and its branches."""
        name = u["name"]
        runs = ([u["run"]] if u["run"] else []) + [b[2] for b in u["branches"]]
        if stop.is_set():
            return [(r, None, None, None) for r in runs]
        res = out / (f"{u['run']}.json" if u["run"] else f"_{name}.json")
        for r in runs + ([] if u["run"] else [res.stem]):
            if (out / f"{r}.json").exists():
                (out / f"{r}.json").unlink()
        # synthetic cases (game/verify/<ADDR>.syn; verify.cpp load_synth)
        env = {**os.environ, "GENESIS_NO_LAUNCHER": "1", "SDL_AUDIODRIVER": "dummy", "XMEN2_VERIFY_OUT": str(res),
               "XMEN2_VERIFY_SYNTH": str(common.GAME / "verify"), "XMEN2_VERIFY_INSNS": str(insns), **crash_env(src)}
        if a.functions:
            env["XMEN2_VERIFY_ONLY"] = a.functions
        cmd = [src.name, str(common.ROM), "--benchmark", str(u["frames"]),
               "--input-script", str(script_of(u["input"])), "--fresh-evidence"]
        if states:   # state files: every process of the group with one /GS cookie
            env["XMEN2_GS_COOKIE"] = states.cookie
        if u.get("states"):   # the runner's --state-at: FRAME PATH
            cmd += ["--state-at", str(states.plan(u["states"], out / f"{name}.states"))]
        if "load" in u:
            cmd += ["--state-load", str(states.path(u["load"]))]
        if u["branches"]:   # the runner's --branch-plan: FRAME FRAMES TAG SCRIPT
            plan = out / f"{name}.branches"
            plan.write_text("".join(f"{at} {n} {run} {script_of(inp)}\n" for at, n, run, inp in u["branches"]),
                            newline="\n")
            cmd += ["--branch-plan", str(plan)]
        logf = out / f"{name}.log"
        run_dir = free.get()
        t0 = time.time()
        timed_out = False
        try:
            if stop.is_set():
                return [(r, None, None, None) for r in runs]
            cmd[0] = str(run_dir / src.name)
            log(f"verify {name}: {u['frames']} frames" + (f" + {len(u['branches'])} branches "
                                                          f"({sum(b[1] for b in u['branches'])} frames)"
                                                          if u["branches"] else "")
                + (f", {len(u['states'])} states" if u.get("states") else "")
                + (f" from frame {u['load']}" if "load" in u else ""))
            with open(logf, "w", encoding="utf-8", errors="replace") as lf:
                p = subprocess.Popen(cmd, cwd=run_dir, env=env, stdout=subprocess.PIPE, stderr=lf, text=True,
                                     errors="replace")
                with lock:
                    live[name] = p
                try:
                    stdout, _ = p.communicate(timeout=a.timeout)
                except subprocess.TimeoutExpired:
                    p.kill()
                    stdout, _ = p.communicate()
                    timed_out = True
                finally:
                    with lock:
                        live.pop(name, None)
        finally:
            free.put(run_dir)
            if u.get("states"):
                states.producer_done.set()
        secs = round(time.time() - t0, 1)
        if name in killed:
            return [(r, None, None, secs) for r in runs]
        if timed_out:
            err = fail(name, f"verify {name} ({', '.join(runs)}): timed out after {a.timeout} s; see {logf}")
            return [(runs[0], None, err, secs)] + [(r, None, None, secs) for r in runs[1:]]
        if p.returncode != 0:
            data = json.loads(res.read_text()) if res.exists() else {}
            why = data.get("stopped_early")
            text = logf.read_text(errors="replace")
            last = [l for l in text.splitlines() if l.startswith(("[handc]", "[verify]", "[branch]"))]
            logf.with_suffix(".stdout").write_text(stdout or "", encoding="utf-8")   # the runner's own lines too
            err = fail(name, f"verify {name} ({', '.join(runs)}): the verification runner failed ({p.returncode})"
                             f"{': ' + (why or last[-1]) if why or last else ''}{crash_note(text)}; see {logf}")
            return [(runs[0], None, err, secs)] + [(r, None, None, secs) for r in runs[1:]]
        got = []
        bench = re.search(r"GENESISRECOMP_BENCHMARK (\{.*\})", stdout or "")
        if u["run"]:
            data = json.loads(res.read_text()) if res.exists() else None
            got.append(check(u["run"], data, logf, json.loads(bench.group(1)) if bench else None, u["frames"],
                             secs - sum(b.get("seconds", 0) for b in
                                        map(json.loads, re.findall(r"GENESISRECOMP_BRANCH (\{.*\})", stdout or "")))))
        elif not bench or not json.loads(bench.group(1)).get("complete"):
            err = fail(name, f"verify {name}: the runner did not play its {u['frames']} frames; see {logf}")
            return [(runs[0], None, err, secs)] + [(r, None, None, secs) for r in runs[1:]]
        played = {b["tag"]: b for b in map(json.loads, re.findall(r"GENESISRECOMP_BRANCH (\{.*\})", stdout or ""))}
        for _at, n, run, _inp in u["branches"]:
            rj = out / f"{run}.json"
            data = json.loads(rj.read_text()) if rj.exists() else None
            pb = played.get(run)
            got.append(check(run, data, logf, pb, n, round(pb["seconds"], 1) if pb else None))
        return got

    merged: dict[str, dict] = {}
    per_run: dict[str, dict] = {}
    # the longest processes start first, so the last ones to finish are short; the merge stays
    # in run order
    order = units if states else sorted(units, key=lambda u: -u["cost"])   # state_units: already in order
    gate = common.MemGate()

    def gated(u: dict) -> list[tuple]:
        if "load" in u and not states.wait(u["load"], stop):   # a branch: its state first, outside the gate
            if stop.is_set():
                return [(u["run"], None, None, None)]
            return [(u["run"], None, fail(u["run"], f"verify {u['run']}: the TAS run ended before saving frame "
                                                     f"{u['load']}; see {out / (units[0]['name'] + '.log')}"), None)]
        with gate:
            return one(u)

    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
        futs = [ex.submit(gated, u) for u in order]
        by_run = {r[0]: r for f in futs for r in f.result()}
    if states:
        states.done()
    results = [by_run[p.stem] for p in inputs]
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
    processes = [{"name": u["name"], "input": u["input"].stem, "frames": u["frames"], "run": u["run"],
                  "branches": [f"{run}@{at}" for at, _n, run, _i in u["branches"]], "cost": u["cost"],
                  **({"states": u["states"]} if u.get("states") else {}),
                  **({"loaded_at": u["load"]} if "load" in u else {})} for u in units]
    return {"runs": len(inputs), "per_run": per_run, "processes": processes, "functions": funcs}


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
Game controllers (players 1-4, in the order they are plugged in): D-pad or left stick; X = A,
A = B, B = C, Start; LB = X, Y = Y, RB = Z, Back = Mode.  F11: fullscreen.  Esc: quit.

Enhancements, each off unless you turn it on:
    full_rate_2p=1      two players at the full 60 frames a second (the original halves it)
    high_fps=144        frames in between the game's own, for a 120/144/165/240 Hz display
    widescreen=16:9     a 16:9 view in the stages (the Cerebro screens stay 4:3); the game
                        plays as in 4:3, so enemies still appear at the old screen edge
    widescreen_gameplay=1  with widescreen: enemies wake and appear across the wide view, and
                        the camera keeps the wide view inside the stage (changes the game)
    players=4          three or four players (players=3 or 4; the original: 2): players 3
                        and 4 join and choose at the player select as player 2 does, have
                        their own panels, and the camera keeps everyone in view; their
                        characters' music parts play on a second sound machine
Sound, on unless you turn it off:
    music_voices=0      the original's single set of sound chips. By default the music plays on
                        sound chips of its own, players 1 and 2's music parts on a further set,
                        so no part or sound effect takes its notes (on the cartridge, two
                        players' parts need more voices than the chip has, and effects take
                        voices from the music); the game itself is the same either way
Cheats, also off unless you turn them on:
    lives=inf           lives are never spent (or lives=N: start a new game with N, 1-99)
    magneto=1           Magneto can be chosen from the start (the original: from Bani Maza)
    start_char=magneto  the character you start Siberia with (beast, cyclops, gambit,
                        nightcrawler, psylocke, wolverine, magneto; the original: at random)
Gameplay tweaks, off unless you turn them on:
    magneto_flight=1    Magneto moves left and right while hovering, at his walking speed
                        (the original: he can only turn)
on the command line:  {exe} path\\to\\xmen2.bin --options widescreen=16:9,high_fps=144
or in xmen2.ini next to the program:
    [options]
    widescreen=16:9
While playing: F9 turns the in-between frames on and off, F10 the 16:9 view (from the next
screen).  {exe} --options --json lists them.

Online (rollback netplay, two to four players, each on their own machine with their own
cartridge file). The host (seat 0) chooses the options that change the game (players,
widescreen_gameplay, full_rate_2p, the cheats and tweaks) and every seat plays with them;
high_fps, music_voices and the 16:9 view are each seat's own (F9 and F10 work), except that
with the host's widescreen_gameplay=1 the view is the host's too.
    host:   {exe} xmen2.bin --netplay lan --seat 0 --seats 2 --bind 0.0.0.0:7000 --options players=2
    others: {exe} xmen2.bin --netplay lan --seat 1 --seats 2 --bind 0.0.0.0:7001 --peer HOST:7000
lan / direct IP: every seat reaches the host's address (a forwarded UDP port across the
internet).  hub: the host relays the others' traffic (3-4 seats).  ice: NAT traversal through a
public STUN server, the seats swapping their connection details as files (--ice-signal-dir DIR,
a shared folder).  --input-delay D (frames, default 5).  {exe} --netplay --json lists them.

Recording: --record-inputs FILE writes every frame's pads when you quit (Esc), online too (the
shared game, the same file on every seat). It is small, it holds no picture or sound, and the game
can replay it exactly, so a session can be turned into video afterwards.

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


def wsl_path(p: Path) -> str:
    s = str(p.resolve()).replace("\\", "/")
    return f"/mnt/{s[0].lower()}{s[2:]}" if re.match(r"^[A-Za-z]:", s) else s


def wsl_run(cmd: str, timeout: int = 3600) -> subprocess.CompletedProcess:
    return subprocess.run(["wsl", "--", "bash", "-lc", cmd], capture_output=True, text=True,
                          errors="replace", timeout=timeout)


def package_licenses(dest: Path, sdl: bool) -> None:
    """The licenses of what the native runner is built from, and this repo's own (NOTICE.md lists
    them), into a package's LICENSES/. libjuice's comes from the build tree (CMake fetches it), so the
    caller copies that one (package_licenses_libjuice)."""
    engine = common.ENGINE / "runner" / "external"
    pairs = [(common.ROOT / "LICENSE", "xmen2-pc-LICENSE.txt"), (common.ROOT / "NOTICE.md", "xmen2-pc-NOTICE.md"),
             (common.ENGINE / "LICENSE.md", "segagenesisrecomp-LICENSE.md"),
             (common.ENGINE / "THIRD-PARTY-LICENSES.md", "segagenesisrecomp-THIRD-PARTY-LICENSES.md"),
             (engine / "ymfm" / "LICENSE", "ymfm-LICENSE.txt"),
             (engine / "superzazu" / "LICENSE", "superzazu-z80-LICENSE.txt"),
             (engine / "minicoro" / "LICENSE", "minicoro-LICENSE.txt"),
             (common.ENGINE / "external" / "recomp-net" / "LICENSE", "recomp-net-LICENSE.txt")]
    if sdl:
        pairs.append((engine / "SDL2" / "SDL2-2.28.5" / "COPYING.txt", "SDL2-COPYING.txt"))
    for src, name in pairs:
        shutil.copy2(src, dest / name)


def linux_native_build() -> str:
    """`native-build` in this machine's WSL Ubuntu (P9), build dir ~/xmen2-native-build/<hash of the
    tree> (one per source tree); returns that directory (holding XMen2Native)."""
    import hashlib
    root = wsl_path(common.ROOT)
    bdir = "$HOME/xmen2-native-build/" + hashlib.sha1(root.encode()).hexdigest()[:10]
    r = wsl_run(f"cd '{root}' && XMEN2_BUILD={bdir} python3 tools/xmen2check.py native-build --json")
    j = next((json.loads(l) for l in reversed(r.stdout.splitlines()) if l.startswith("{")), None)
    if r.returncode or not j or not j.get("ok"):
        sys.exit(f"WSL native-build failed: {(r.stdout + r.stderr).strip()[-600:]}")
    return bdir


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
    wsl = wsl_run
    bdir = linux_native_build()
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
    package_licenses(stage / "LICENSES", sdl=False)
    c = wsl(f"cp \"{bdir}/_deps/libjuice-src/LICENSE\" '{wsl_path(stage)}/LICENSES/libjuice-LICENSE.txt'")
    if c.returncode:
        sys.exit(f"libjuice's license (MPL-2.0) is not in the Linux build tree: {c.stderr.strip()[-300:]}")
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
    n = wsl(f"{x} --netplay --json", timeout=120)
    netplay = next((json.loads(l) for l in reversed(n.stdout.splitlines()) if l.startswith("{")), None)
    ok = (nwin == 0 and smoke["rom"]["exit"] == 0 and smoke["rom"]["benchmark"] and smoke["wrong_rom"]["exit"] == 2
          and smoke["wrong_rom"]["refused"] and t.returncode == 0 and sync.is_file())
    return {"ok": ok, "platform": "linux", "zip": str(zpath),
            "files": [p.relative_to(stage).as_posix() for p in files], "rom_windows": nwin,
            "rom_window_hits": {k: v for k, v in windows.items() if v}, "smoke": smoke,
            "tas_sync": str(sync), "tas_frames": frames, "tas_exit": t.returncode, "options": opts,
            "netplay": netplay if n.returncode == 0 else None}


def free_udp_ports(n: int) -> list[int]:
    import socket
    socks, ports = [], []
    for _ in range(n):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.bind(("0.0.0.0", 0))
        socks.append(s)
        ports.append(s.getsockname()[1])
    for s in socks:
        s.close()
    return ports


def parse_kv(text: str) -> dict[str, str]:
    return dict(kv.split("=", 1) for kv in text.split(",") if "=" in kv)


def cmd_netplay(a) -> dict:
    """Phase 7 (P7-multiplayer.md "The contract"): N processes of the release runner on loopback (or a
    Windows/WSL pair), seat i playing pad column i+1 of --input, seat 0 hosting (--options, --session
    and --scene are the host's: the guests take them from the session's seal, R10; --seat-options
    I:K=V,... is seat I's own command line; --seat-f10 I:TICK,... presses a seat's F10, R12). Each seat writes OUT/seat<i>/sync.csv and digest.csv (its
    final timeline) and its seat.json; the link simulator (RNET_SIM_*) runs on every seat's receive
    path. Prints {"ok", "seats": [...], "link", "transport"}."""
    import concurrent.futures
    import os
    import platform
    import time
    if a.seats < 2 or a.seats > 4:
        sys.exit("--seats: 2 to 4")
    transport = a.transport or ("lan" if a.seats == 2 else "hub")
    if transport == "lan" and a.seats != 2:
        sys.exit("--transport lan: two seats (hub for more)")
    common.ensure_rom()
    src = exe_path(NATIVE_EXE)
    if not src.is_file():
        sys.exit(f"no Release runner at {src}: run `xmen2check.py build`")
    out = Path(a.out).resolve()
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True)
    script = out / "input.input"
    script.write_text("".join(l for l in Path(a.input).read_text().splitlines(True)
                              if not l.strip().upper().startswith("EXIT")), newline="\n")
    frames = script_frames(script)
    linux = {int(x) for x in a.linux_seats.split(",") if x.strip()} if a.linux_seats else set()
    own: dict[int, str] = {}
    for so in a.seat_options or []:
        i, kv = so.split(":", 1)
        own[int(i)] = kv
    link = parse_kv(a.link) if a.link else {}
    ports = free_udp_ports(a.seats)
    wsl_ip = gateway = None
    linux_dir = None
    if linux:
        linux_dir = linux_native_build()
        r = wsl_run("hostname -I; ip route show default")
        words = r.stdout.split()
        wsl_ip = words[0] if words else None
        gateway = words[words.index("via") + 1] if "via" in words else None
        if not wsl_ip or not gateway:
            sys.exit(f"cannot find WSL's address: {r.stdout!r}")

    def addr(i: int, seen_from_linux: bool) -> str:
        if i in linux:
            return f"{'127.0.0.1' if seen_from_linux else wsl_ip}:{ports[i]}"
        return f"{gateway if seen_from_linux and linux else '127.0.0.1'}:{ports[i]}"

    def seat_cmd(i: int) -> tuple[list[str], dict, Path]:
        d = out / f"seat{i}"
        d.mkdir()
        on_linux = i in linux
        args = ["--benchmark", str(frames), "--netplay", transport, "--seat", str(i), "--seats", str(a.seats),
                "--bind", f"{'0.0.0.0' if linux else '127.0.0.1'}:{ports[i]}"]
        if transport == "ice":   # signals through files (no lobby, R8); the addresses come from ICE
            args += ["--ice-signal-dir", wsl_path(out) if on_linux else str(out)]
        elif i:
            args += ["--peer", addr(0, on_linux)]
        elif transport == "lan":   # the host dials its guest too: a first packet out opens the path back
            args += ["--peer", addr(1, on_linux)]
        opts = []
        if i == 0:
            opts += [a.options] if a.options else []
            args += (["--session", str(a.session)] if a.session is not None else []) + \
                    (["--scene", str(a.scene)] if a.scene is not None else [])
        if i in own:
            opts.append(own[i])
        if opts:
            args += ["--options", ",".join(opts)]
        env = {"SDL_AUDIODRIVER": "dummy", "GENESIS_NO_LAUNCHER": "1"}
        for k, e in (("latency", "RNET_SIM_LATENCY_MS"), ("jitter", "RNET_SIM_JITTER_MS"), ("loss", "RNET_SIM_LOSS_PCT")):
            if k in link:
                env[e] = link[k]
        if "seed" in link:
            env["RNET_SIM_SEED"] = str(int(link["seed"]) + i)
        if a.seat_f10:
            env["XMEN2_NP_F10"] = a.seat_f10
        if on_linux:
            p = lambda q: wsl_path(q)   # noqa: E731
            env["XMEN2_SYNC_CSV"] = p(d / "sync.csv")
            args += ["--input-script", p(script), "--digest", p(d / "digest.csv"), "--netplay-json", p(d / "seat.json")]
            envs = " ".join(f"{k}='{v}'" for k, v in env.items())
            sh = f"cd '{p(d)}' && {envs} \"{linux_dir}/XMen2Native\" '{p(common.ROM)}' " + " ".join(f"'{x}'" for x in args)
            return ["wsl", "--", "bash", "-lc", sh], dict(os.environ), d
        run_dir = d / "runner"
        run_dir.mkdir()
        copy_runner(src, run_dir)
        env["XMEN2_SYNC_CSV"] = str(d / "sync.csv")
        args += ["--input-script", str(script), "--digest", str(d / "digest.csv"), "--netplay-json", str(d / "seat.json")]
        return [str(run_dir / src.name), str(common.ROM), *args], {**os.environ, **env}, d

    log(f"netplay: {a.seats} seats, {transport}, {frames} frames, link {a.link or 'clean'}")
    procs = []
    for i in range(a.seats):
        cmd, env, d = seat_cmd(i)
        lf = open(d / "runner.log", "w", encoding="utf-8", errors="replace")
        procs.append((i, subprocess.Popen(cmd, cwd=d, env=env, stdout=subprocess.PIPE, stderr=lf, text=True), d, lf))
    deadline = time.time() + a.timeout
    seats = []
    for i, p, d, lf in procs:
        try:
            p.wait(timeout=max(1, deadline - time.time()))
        except subprocess.TimeoutExpired:
            p.kill()   # this process only (by its handle)
            p.wait()
        lf.close()
        j = {}
        if (d / "seat.json").is_file():
            j = json.loads((d / "seat.json").read_text())
        seats.append({"seat": i, "os": "linux" if i in linux else platform.system().lower(), "exit": p.returncode,
                      "sync_csv": str(d / "sync.csv"), "digest_csv": str(d / "digest.csv"),
                      "options": j.get("options"), "rollbacks": j.get("rollbacks"), "max_rollback": j.get("max_rollback"),
                      "late_frames": j.get("late_frames"), "frames": j.get("frames"), "episodes": j.get("episodes"),
                      "replayed_ticks": j.get("replayed_ticks"), "log": str(d / "runner.log")})
        if p.returncode == 0 and not a.keep_runner:
            shutil.rmtree(d / "runner", ignore_errors=True)
    return {"ok": all(s["exit"] == 0 for s in seats), "seats": seats, "frames": frames,
            "link": link, "transport": transport}


def runner_options(cmd: list[str], cwd: Path | None = None) -> dict:
    """The runner's own `--options --json` (phase 6)."""
    r = subprocess.run([*cmd, "--options", "--json"], capture_output=True, text=True, timeout=120, cwd=cwd)
    j = next((json.loads(l) for l in reversed(r.stdout.splitlines()) if l.startswith("{")), None)
    if r.returncode or not j or "options" not in j:
        sys.exit(f"{cmd[-1]} --options --json failed ({r.returncode}): {(r.stdout + r.stderr).strip()[-300:]}")
    return j


def runner_netplay(cmd: list[str], cwd: Path | None = None) -> dict | None:
    """The runner's own `--netplay --json` (phase 7): its transports and seats; None if it has none."""
    r = subprocess.run([*cmd, "--netplay", "--json"], capture_output=True, text=True, timeout=120, cwd=cwd)
    j = next((json.loads(l) for l in reversed(r.stdout.splitlines()) if l.startswith("{")), None)
    return j if r.returncode == 0 and j and "transports" in j else None


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
    stage = BUILD / "package" / "xmen2-native-windows"
    if stage.exists():
        shutil.rmtree(stage)
    (stage / "LICENSES").mkdir(parents=True)
    shutil.copy2(exe, stage / "XMen2.exe")
    shutil.copy2(exe.parent / "SDL2.dll", stage / "SDL2.dll")
    (stage / "README.txt").write_text(PACKAGE_README.format(exe="XMen2.exe"), encoding="utf-8", newline="\r\n")
    package_licenses(stage / "LICENSES", sdl=True)
    juice = BUILD / "_deps" / "libjuice-src" / "LICENSE"
    if not juice.is_file():
        sys.exit(f"libjuice's license (MPL-2.0) is not in the build tree ({juice})")
    shutil.copy2(juice, stage / "LICENSES" / "libjuice-LICENSE.txt")
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
            "options": runner_options([str(stage / "XMen2.exe")]),
            "netplay": runner_netplay([str(stage / "XMen2.exe")])}


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
                   help="runner processes at once (each on its own runner copy; ~0.2 GB and a core each); a "
                        "process starts while $XMEN2_MIN_FREE_GB (default 3) of RAM is free")
    p.add_argument("--timeout", type=int, default=7200)
    p.add_argument("--out", default="", help="output directory (default build/verify)")
    p.add_argument("--keep-going", action="store_true", help="play every run even after one failed")
    p.add_argument("--keep-runner", action="store_true", help="keep the runner copies after a pass (XMEN2_KEEP_RUNNER=1)")
    p.add_argument("--no-branch", action="store_true",
                   help="play every run from power-on (the control: by default runs that start with the TAS's "
                        "input branch from a TAS run at the frame they leave it)")
    p.add_argument("--in-process", action="store_true",
                   help="branches inside TAS processes (--branch-plan), not from the TAS run's state files (the control)")
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
            p.add_argument("--render", action="store_true",
                           help="stepped modes: render every sync point's frame even when nothing compares the "
                                "picture (the control for --no-render, see picture_free)")
            p.add_argument("--timeout", type=int, default=7200)
            p.add_argument("--dump-frames", default="", help="dev: dump full state after these frames")
            p.add_argument("--keep-runner", action="store_true",
                           help="keep OUT/runner (the private runner copy) after a pass (XMEN2_KEEP_RUNNER=1)")
            p.add_argument("--options", default="", help="native (phase 6): the runner's --options K=V,...")
            p.add_argument("--present-log", action="store_true",
                           help="native (phase 6 E4): OUT/present.csv, one row per displayed frame")
            p.add_argument("--flip", type=int, default=0,
                           help="native (phase 6 E4): player 1's C inverted in runner frames N to N+7")
            p.add_argument("--flip-scan", default="",
                           help="native (phase 6 E4) LO:HI:L: before each runner frame N in [LO, HI) save, run "
                                "N..N+L with --flip N into OUT/flip_scan.csv, restore (with --present-log)")
            p.add_argument("--flip-scan-first", action="store_true",
                           help="with --flip-scan: stop after the first N (a gameplay sync point) whose flip changes "
                                "the picture against the run's own unflipped rows")
            p.add_argument("--game-rows", action="store_true",
                           help="native (phase 6 R8): OUT/game.csv, the game's state at each sync point less "
                                "what only the picture depends on")
            p.add_argument("--session", type=int, default=None,
                           help="native (phase 7): the session machine with this seed (P7 \"Definitions\")")
            p.add_argument("--scene", type=int, default=None, help="native (phase 7): a new game starts at scene N")
            p.add_argument("--digest", action="store_true", help="native (phase 7): OUT/digest.csv, the canonical digest")
            p.add_argument("--state-probe", default="",
                           help="native (phase 7 M2) P:L: every P-th sync point save, run L, restore, run L again")
            p.add_argument("--state-skip", default="",
                           help="native (phase 7 M2): leave this section out of every restore (negative control)")
    p = sub.add_parser("trace-group")   # many native traces, the fuzz runs as branches of TAS runs
    p.add_argument("--json", action="store_true")
    p.add_argument("--mode", default="native")
    p.add_argument("--runs", required=True, help='JSON list of {"run", "input", "replay", "out"}')
    p.add_argument("--tas-input", default="", help="the TAS's .input, when its run is not in --runs")
    p.add_argument("--tas-replay", default="", help="the TAS's D1 trace dir, when its run is not in --runs")
    p.add_argument("--jobs", type=int, default=0, help="processes at once (default: the cores less two)")
    p.add_argument("--no-branch", action="store_true", help="every run from power-on (the control)")
    p.add_argument("--in-process", action="store_true",
                   help="branches inside TAS processes (--branch-plan), not from the TAS run's state files (the control)")
    p.add_argument("--timeout", type=int, default=7200)
    p.add_argument("--keep-runner", action="store_true")
    p.add_argument("--fb", action="store_true")
    p.add_argument("--render", action="store_true")
    p.add_argument("--options", default="")
    p.add_argument("--present-log", action="store_true")
    p.add_argument("--game-rows", action="store_true")
    p.add_argument("--session", type=int, default=None)
    p.add_argument("--scene", type=int, default=None)
    p.add_argument("--digest", action="store_true")
    p.add_argument("--state-probe", default="")
    p.add_argument("--state-skip", default="")
    p.set_defaults(flip=0, flip_scan="", flip_scan_first=False)
    p = sub.add_parser("options")
    p.add_argument("--json", action="store_true")
    p = sub.add_parser("netplay")   # phase 7 (P7-multiplayer.md "The contract")
    p.add_argument("--json", action="store_true")
    p.add_argument("--seats", type=int, required=True)
    p.add_argument("--input", required=True, help="the pads by runner frame: seat i plays column i+1")
    p.add_argument("--scene", type=int, default=None)
    p.add_argument("--session", type=int, default=None)
    p.add_argument("--options", default="", help="the host's options (sealed into the session)")
    p.add_argument("--seat-options", action="append", help="I:K=V,...: seat I's own command line")
    p.add_argument("--seat-f10", default="", help="I:TICK,...: seat I presses F10 before that live tick (R12)")
    p.add_argument("--link", default="", help="latency=L,jitter=J,loss=P,seed=S (the link simulator)")
    p.add_argument("--transport", default="", choices=["", "lan", "hub", "ice"])
    p.add_argument("--linux-seats", default="", help="seats run in WSL (the Linux build)")
    p.add_argument("--out", required=True)
    p.add_argument("--timeout", type=int, default=3600)
    p.add_argument("--keep-runner", action="store_true")
    p = sub.add_parser("package")
    p.add_argument("--json", action="store_true")
    p.add_argument("--platform", default="windows", choices=["windows", "linux"])
    p = sub.add_parser("native-build")
    p.add_argument("--json", action="store_true")
    a = ap.parse_args()
    res = {"gen": cmd_gen, "build": cmd_build, "trace": cmd_trace, "trace-group": cmd_trace_group, "types": cmd_types,
           "handc": cmd_handc, "lint": cmd_lint, "verify": cmd_verify, "package": cmd_package,
           "native-build": cmd_native_build, "options": cmd_options, "netplay": cmd_netplay}[a.cmd](a)
    if a.cmd == "gen" and not a.json:
        res = {k: v for k, v in res.items() if k != "functions"}
    print(json.dumps(res, indent=None if a.json else 1))
    # a failed build is a failed command (a link over a running runner exited 0 and left the old
    # exe: 2026-10-21); the other commands' "ok" is a result their callers grade
    return 1 if a.cmd in ("build", "native-build") and res.get("ok") is False else 0


if __name__ == "__main__":
    raise SystemExit(main())
