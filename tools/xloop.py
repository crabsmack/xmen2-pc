"""One first-divergence iteration (exact mode): frame CRCs first, then instructions.

    uv run tools/xloop.py --to 600 [--run tas] [--window 2] [--no-build]

1. Builds the Release runner (unless --no-build) and plays frames 1..TO of RUN with the
   per-frame CRC trace; finds the first frame whose RAM/VRAM/CRAM/VSRAM CRC differs from the
   reference (xmen2disasm emu/runs/RUN/frames.csv).
2. Traces the WINDOW frames ending there at instruction level on both sides (runner:
   GENESIS_XTRACE; reference: xmen2disasm emu/cycle_trace.py) and prints the first differing
   instruction (tools/ctrace_diff.py). If the window's first row already differs, the cause
   is earlier: rerun with a larger --window.
"""
from __future__ import annotations

import argparse
import csv
import os
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

DIS_MAIN = common.ROOT.parent / "xmen2disasm"
COLS = ["ram_crc32", "vram_crc32", "cram_crc32", "vsram_crc32"]


def run(cmd, cwd, **kw):
    return subprocess.run(cmd, cwd=cwd, **kw)


def first_divergence(got_csv: Path, ref_csv: Path) -> tuple[int | None, int]:
    with open(ref_csv, newline="") as f:
        ref = {int(r["frame"]): r for r in csv.DictReader(f)}
    n = 0
    with open(got_csv, newline="") as f:
        for r in csv.DictReader(f):
            fr = int(r["frame"])
            if fr not in ref:
                continue
            n += 1
            if any(r[c] != ref[fr][c] for c in COLS):
                return fr, n
    return None, n


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--run", default="tas")
    ap.add_argument("--to", dest="f1", type=int, required=True)
    ap.add_argument("--window", type=int, default=2)
    ap.add_argument("--no-build", action="store_true")
    ap.add_argument("--disasm", default=str(DIS_MAIN), help="xmen2disasm checkout (reference runs + emu tools)")
    a = ap.parse_args()
    out = common.BUILD / "xloop"
    out.mkdir(parents=True, exist_ok=True)
    if not a.no_build:
        r = run(["uv", "run", "tools/xmen2check.py", "build"], common.ROOT, capture_output=True, text=True)
        if '"ok": true' not in r.stdout:
            for line in open(common.BUILD / "build.log", errors="replace"):
                if " error " in line or "error C" in line or "error LNK" in line:
                    print(line.rstrip())
            return 2
    inp = out / f"{a.run}.input"
    run(["uv", "run", "emu/export_input.py", a.run, "--out", str(inp)], a.disasm, check=True,
        capture_output=True)
    ref_frames = Path(a.disasm) / "emu" / "runs" / a.run / "frames.csv"

    def trace(frames: int, xtrace: str | None) -> Path:
        env = dict(os.environ)
        if xtrace:
            env["GENESIS_XTRACE"] = xtrace
        tdir = out / "trace"
        r = run(["uv", "run", "tools/xmen2check.py", "trace", "--run", a.run, "--input", str(inp),
                 "--out", str(tdir), "--frames", str(frames)], common.ROOT, env=env,
                capture_output=True, text=True)
        if r.returncode:
            print(r.stdout[-600:], r.stderr[-800:])
            log = tdir / "runner.log"
            if log.exists():
                print("".join(open(log, errors="replace").readlines()[-12:]))
        return tdir / "frames.csv"

    got = trace(a.f1, None)
    fr, n = first_divergence(got, ref_frames)
    if fr is None:
        print(f"frames 1..{n}: RAM/VRAM/CRAM/VSRAM equal on every frame")
        return 0
    print(f"first CRC divergence: frame {fr}")
    f0 = max(1, fr - a.window + 1)
    ref = out / f"ref-{a.run}-{f0}-{fr}.csv"
    if not ref.exists():
        run(["uv", "run", "emu/cycle_trace.py", a.run, "--from", str(f0), "--to", str(fr), "--out", str(ref)],
            a.disasm, check=True, capture_output=True)
    game = out / f"game-{a.run}-{f0}-{fr}.csv"
    trace(fr + 1, f"{f0},{fr},{game}")
    return run(["uv", "run", "tools/ctrace_diff.py", str(game), str(ref)], common.ROOT).returncode


if __name__ == "__main__":
    raise SystemExit(main())
