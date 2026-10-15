"""Compare two sync.csv files (phase 4 D1/D2): first sync point whose CRCs differ.

    uv run tools/synccmp.py REF/sync.csv GOT/sync.csv
"""
from __future__ import annotations

import csv
import sys

COLS = ["ram_crc32", "vram_crc32", "cram_crc32", "vsram_crc32"]


def load(p: str) -> dict[int, dict[str, str]]:
    with open(p, newline="") as f:
        return {int(r["sync"]): r for r in csv.DictReader(f)}


def main() -> int:
    ref, got = load(sys.argv[1]), load(sys.argv[2])
    first = {}
    for s in sorted(ref):
        g = got.get(s)
        if g is None:
            print(f"sync {s} (frame {ref[s]['frame']}): missing from {sys.argv[2]}")
            return 1
        for c in COLS:
            if c not in first and g[c] != ref[s][c]:
                first[c] = (s, ref[s]["frame"])
    for c in COLS:
        print(f"{c:12} first divergence: " + (f"sync {first[c][0]} (frame {first[c][1]})" if c in first else "None"))
    extra = len(got) - len(ref)
    if extra:
        print(f"{extra:+d} sync points vs the reference")
    print(f"{len(ref)} sync points " + ("equal" if not first and not extra else "DIFFER"))
    return 0 if not first and not extra else 1


if __name__ == "__main__":
    raise SystemExit(main())
