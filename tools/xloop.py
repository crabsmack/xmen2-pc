"""One first-divergence iteration at instruction level (exact mode).

    uv run tools/xloop.py --to 60 [--from 1] [--run tas] [--no-build]

Builds the Release runner, traces frames FROM..TO on both sides (runner: GENESIS_XTRACE;
reference: xmen2disasm emu/cycle_trace.py, cached in build/xloop/), diffs the instruction
streams (tools/ctrace_diff.py) and, past the last matching instruction, the per-frame CRCs.
"""
from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

DIS_MAIN = common.ROOT.parent / "xmen2disasm"


def run(cmd, cwd, **kw):
    return subprocess.run(cmd, cwd=cwd, **kw)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--run", default="tas")
    ap.add_argument("--from", dest="f0", type=int, default=1)
    ap.add_argument("--to", dest="f1", type=int, required=True)
    ap.add_argument("--no-build", action="store_true")
    ap.add_argument("--disasm", default=str(DIS_MAIN), help="xmen2disasm checkout with emu/cycle_trace.py")
    a = ap.parse_args()
    out = common.BUILD / "xloop"
    out.mkdir(parents=True, exist_ok=True)
    if not a.no_build:
        r = run(["uv", "run", "tools/xmen2check.py", "build"], common.ROOT, capture_output=True, text=True)
        if '"ok": true' not in r.stdout:
            print(r.stdout[-400:])
            for line in open(common.BUILD / "build.log", errors="replace"):
                if "error" in line and ("C2" in line or "LNK" in line or "C4013" in line):
                    print(line.rstrip())
            return 2
    inp = out / f"{a.run}.input"
    if not inp.exists():
        run(["uv", "run", "emu/export_input.py", a.run, "--out", str(inp)], a.disasm, check=True)
    ref = out / f"ref-{a.run}-{a.f0}-{a.f1}.csv"
    if not ref.exists():
        run(["uv", "run", "emu/cycle_trace.py", a.run, "--from", str(a.f0), "--to", str(a.f1),
             "--out", str(ref)], a.disasm, check=True, capture_output=True)
    game = out / f"game-{a.run}-{a.f0}-{a.f1}.csv"
    env = {**__import__("os").environ, "GENESIS_XTRACE": f"{a.f0},{a.f1},{game}"}
    r = run(["uv", "run", "tools/xmen2check.py", "trace", "--run", a.run, "--input", str(inp),
             "--out", str(out / "trace"), "--frames", str(a.f1)], common.ROOT, env=env,
            capture_output=True, text=True)
    if r.returncode:
        print(r.stdout[-600:], r.stderr[-1500:])
        log = out / "trace" / "runner.log"
        if log.exists():
            print("".join(open(log, errors="replace").readlines()[-15:]))
    d = run(["uv", "run", "tools/ctrace_diff.py", str(game), str(ref)], common.ROOT)
    run(["uv", "run", "tools/compare.py", str(out / "trace" / "frames.csv"), "--ref", a.run, "--show", "3"],
        common.ROOT)
    return d.returncode


if __name__ == "__main__":
    raise SystemExit(main())
