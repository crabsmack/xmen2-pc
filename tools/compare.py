"""Compare a trace's frames.csv with a reference run's, frame by frame.

    uv run tools/compare.py build/trace-dev/tas/frames.csv [--ref RUN] [--show 10]

Prints the first divergent frame per compared column and a few rows around the first one.
The reference is xmen2disasm's emu/runs/<RUN>/frames.csv (main checkout).
"""
from __future__ import annotations

import argparse
import csv
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

COLS = ["ram_crc32", "vram_crc32", "cram_crc32", "vsram_crc32", "z80_crc32"]


def ref_runs() -> Path:
    for d in (common.ROOT.parent / "xmen2disasm" / "emu" / "runs", common.DISASM / "emu" / "runs"):
        if (d / "tas" / "frames.csv").exists():
            return d
    sys.exit("no reference runs found (xmen2disasm emu/runs)")


def load(p: Path) -> dict[int, dict[str, str]]:
    with open(p, newline="") as f:
        return {int(r["frame"]): r for r in csv.DictReader(f)}


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("trace")
    ap.add_argument("--ref", default="tas")
    ap.add_argument("--show", type=int, default=6)
    a = ap.parse_args()
    got = load(Path(a.trace))
    ref = load(ref_runs() / a.ref / "frames.csv")
    frames = sorted(f for f in got if f in ref)
    first = {}
    for c in COLS:
        first[c] = next((f for f in frames if got[f][c].lower() != ref[f][c].lower()), None)
        print(f"{c:12} first divergence: {first[c]}")
    gated = [first[c] for c in COLS[:4] if first[c] is not None]
    if not gated:
        print(f"all {len(frames)} frames equal (ram/vram/cram/vsram)")
        return 0
    f0 = min(gated)
    print(f"\nframe  {'  '.join(c[:5] + '(got/ref)' for c in COLS)}")
    for f in range(max(1, f0 - 2), f0 + a.show):
        if f in got and f in ref:
            print(f"{f:5}  " + "  ".join(
                ("=" if got[f][c] == ref[f][c] else "X") + f"{got[f][c]}/{ref[f][c]}" for c in COLS))
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
