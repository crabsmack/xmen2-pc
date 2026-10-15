"""Diff two RAM dumps taken at a sync point (XMEN2_SYNC_DUMP, phase 4), naming the A5 globals.

    uv run tools/syncdiff.py REF/s000661.ram GOT/s000661.ram [--max 40]
"""
from __future__ import annotations

import argparse
import bisect
import csv
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402


def globals_table() -> list[tuple[int, str]]:
    p = common.DISASM_MAIN / "build" / "recomp" / "ram_globals.csv"
    if not p.exists():
        return []
    with open(p, newline="") as f:
        return sorted((int(r["ram_addr"], 16), r["name"]) for r in csv.DictReader(f))


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("ref")
    ap.add_argument("got")
    ap.add_argument("--max", type=int, default=40)
    a = ap.parse_args()
    r, g = Path(a.ref).read_bytes(), Path(a.got).read_bytes()
    glb = globals_table()
    keys = [k for k, _ in glb]
    diffs = [i for i in range(min(len(r), len(g))) if r[i] != g[i]]
    print(f"{len(diffs)} bytes differ")
    runs, start = [], None
    for i in diffs:
        if start is None or i > prev + 4:
            if start is not None:
                runs.append((start, prev))
            start = i
        prev = i
    if start is not None:
        runs.append((start, prev))
    for s, e in runs[:a.max]:
        addr = 0xFF0000 + s
        k = bisect.bisect_right(keys, addr) - 1
        near = f"{glb[k][1]}+${addr - glb[k][0]:X}" if k >= 0 else ""
        print(f"${addr:06X}-${0xFF0000 + e:06X} ({near}): ref {r[s:e + 1].hex()} got {g[s:e + 1].hex()}")
    return 0 if not diffs else 1


if __name__ == "__main__":
    raise SystemExit(main())
