"""Phase-2 contract tool (xmen2disasm docs/phases/P2-recomp.md): report what the build and
the runner actually did. Never fills gaps: every number comes from the recompiler's or the
runner's own output.

    uv run tools/xmen2check.py gen   [--json]   # regenerate C into build/gen-check/, count problems
    uv run tools/xmen2check.py build [--json]   # configure + build the Release runner
    uv run tools/xmen2check.py trace --run NAME --input FILE.input --out DIR [--frames N]
                                     [--mode recomp] [--json]

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


def exe_path() -> Path:
    return BUILD / "Release" / common.EXE_NAME if sys.platform == "win32" else BUILD / common.EXE_NAME


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
    configure()
    rc = cmake("--build", str(BUILD), "--config", "Release", "--target", "XMen2Recomp", "--parallel",
               logfile=BUILD / "build.log")
    exe = exe_path()
    return {"exe": str(exe), "config": "Release", "ok": rc == 0 and exe.is_file(), "log": str(BUILD / "build.log")}


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


def cmd_trace(a) -> dict:
    if a.mode != "recomp":
        sys.exit(f"--mode {a.mode}: phase 2 implements only recomp")
    common.ensure_rom()
    src = exe_path()
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
    frames = a.frames or script_frames(Path(a.input))
    # --benchmark N ends the run after N frames and prints the counters; an EXIT directive
    # would return from the runner before that summary, so the played copy drops it.
    script = out / "input.input"
    script.write_text("".join(l for l in Path(a.input).read_text().splitlines(True)
                              if not l.strip().upper().startswith("EXIT")), newline="\n")
    csv_path = out / "frames.csv"
    env = {**__import__("os").environ, "XMEN2_TRACE_CSV": str(csv_path), "GENESIS_NO_LAUNCHER": "1",
           "SDL_AUDIODRIVER": "dummy"}
    if a.dump_frames:  # first-divergence state dumps (game/xmen2_spec.c maybe_dump)
        (out / "dump").mkdir(exist_ok=True)
        env.update(XMEN2_DUMP_FRAMES=a.dump_frames, XMEN2_DUMP_DIR=str(out / "dump"))
    cmd = [str(run_dir / src.name), str(common.ROM), "--benchmark", str(frames),
           "--input-script", str(script), "--fresh-evidence"]
    log(f"trace {a.run}: {frames} frames")
    with open(out / "runner.log", "w", encoding="utf-8", errors="replace") as lf:
        r = subprocess.run(cmd, cwd=run_dir, env=env, stdout=subprocess.PIPE, stderr=lf, text=True,
                           errors="replace", timeout=a.timeout)
    (out / "runner.stdout").write_text(r.stdout, encoding="utf-8")
    stderr = (out / "runner.log").read_text(errors="replace")
    bench = re.search(r"GENESISRECOMP_BENCHMARK (\{.*\})", r.stdout)
    counters = re.search(r"\[xmen2\] counters (\{.*\})", stderr)
    if r.returncode != 0 or not bench or not counters:
        sys.exit(f"runner failed ({r.returncode}); see {out / 'runner.log'}")
    b, c = json.loads(bench.group(1)), json.loads(counters.group(1))
    rows = sum(1 for _ in open(csv_path)) - 1 if csv_path.exists() else 0
    return {"frames": rows, "dispatch_misses": c["dispatch_misses"],
            "interp_instructions": c["interp_instructions"], "stub_hits": c["stub_hits"],
            "fps": b["fps"], "complete": b["complete"], "exe": str(run_dir / src.name)}


def main() -> int:
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name in ("gen", "build", "trace"):
        p = sub.add_parser(name)
        p.add_argument("--json", action="store_true")
        if name == "trace":
            p.add_argument("--run", required=True)
            p.add_argument("--input", required=True)
            p.add_argument("--out", required=True)
            p.add_argument("--frames", type=int, default=0)
            p.add_argument("--mode", default="recomp")
            p.add_argument("--timeout", type=int, default=7200)
            p.add_argument("--dump-frames", default="", help="dev: dump full state after these frames")
    a = ap.parse_args()
    res = {"gen": cmd_gen, "build": cmd_build, "trace": cmd_trace}[a.cmd](a)
    if a.cmd == "gen" and not a.json:
        res = {k: v for k, v in res.items() if k != "functions"}
    print(json.dumps(res, indent=None if a.json else 1))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
