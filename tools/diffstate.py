"""Diff two state dumps of the same frame (game: xmen2check.py trace --dump-frames; reference:
xmen2disasm emu/dump_state.py), naming RAM addresses with the disassembly's globals.

    uv run tools/diffstate.py GAME_DIR REF_DIR --frame 4 [--max 40]
"""
from __future__ import annotations

import argparse
import bisect
import csv
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

BASES = {"ram": 0xFF0000, "vram": 0, "cram": 0, "vsram": 0, "z80": 0xA00000}


def globals_map() -> tuple[list[int], list[str]]:
    p = common.DISASM / "build" / "recomp" / "ram_globals.csv"
    if not p.exists():
        return [], []
    rows = sorted((int(r["ram_addr"], 16), r["name"]) for r in csv.DictReader(open(p, newline="")))
    return [a for a, _ in rows], [n for _, n in rows]


def name(addr: int, ga: list[int], gn: list[str]) -> str:
    i = bisect.bisect_right(ga, addr) - 1
    if i < 0 or addr - ga[i] > 0x100:
        return ""
    return gn[i] + (f"+${addr - ga[i]:X}" if addr != ga[i] else "")


def runs(a: bytes, b: bytes) -> list[tuple[int, int]]:
    out, i = [], 0
    while i < len(a):
        if a[i] != b[i]:
            j = i
            while j < len(a) and (a[j] != b[j] or (j + 1 < len(a) and a[j + 1] != b[j + 1])):
                j += 1
            out.append((i, j))
            i = j
        i += 1
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("game")
    ap.add_argument("ref")
    ap.add_argument("--frame", type=int, required=True)
    ap.add_argument("--max", type=int, default=40)
    a = ap.parse_args()
    ga, gn = globals_map()
    stem = f"f{a.frame:06d}"
    for kind, base in BASES.items():
        g, r = Path(a.game) / f"{stem}.{kind}", Path(a.ref) / f"{stem}.{kind}"
        if not g.exists() or not r.exists():
            print(f"{kind}: missing ({g.exists()=}, {r.exists()=})")
            continue
        gb, rb = g.read_bytes(), r.read_bytes()
        rr = runs(gb, rb)
        print(f"== {kind}: {sum(e - s for s, e in rr)} bytes differ in {len(rr)} runs")
        for s, e in rr[:a.max]:
            label = name(base + s, ga, gn) if kind == "ram" else ""
            print(f"  ${base + s:06X}..${base + e - 1:06X} {label:28} game {gb[s:e][:16].hex()}  ref {rb[s:e][:16].hex()}")
    for side, d in (("game", a.game), ("ref", a.ref)):
        p = Path(d) / f"{stem}.regs.json"
        if p.exists():
            print(f"{side:4} regs: " + " ".join(f"{k}={v}" for k, v in json.loads(p.read_text()).items()))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
