"""First Z80 instruction where two Z80 traces disagree.

    uv run tools/ztrace_diff.py GAME.csv REF.csv [--context 12]

Both files are `frame,pc,zclk,rh` (game: GENESIS_ZTRACE=from,to,path on the runner in exact
mode; reference: xmen2disasm `uv run emu/cycle_trace.py RUN --from A --to B --out X --z80-out F`).
zclk is master clocks from the start of the frame; rh hashes AF..HL' at the instruction's start.
"""
from __future__ import annotations

import argparse
import itertools


def rows(path):
    with open(path) as f:
        next(f)
        for line in f:
            fr, pc, clk, rh = line.rstrip("\n").split(",")
            yield int(fr), pc.upper(), int(clk), rh.upper()


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("game")
    ap.add_argument("ref")
    ap.add_argument("--context", type=int, default=12)
    a = ap.parse_args()
    hist, n = [], 0
    for g, r in itertools.zip_longest(rows(a.game), rows(a.ref)):
        n += 1
        if g is None or r is None:
            print(f"Z80 trace ends after {n - 1} matching rows ({'game' if g is None else 'ref'} shorter)")
            return 0 if g is None and r is None else 1
        if g[1:] != r[1:] or g[0] != r[0]:
            print(f"first Z80 difference at row {n}:")
            print("   frame  pc         game_zclk  ref_zclk  dclk  regs")
            for hg, hr in hist[-a.context:]:
                print(f"   {hg[0]:5}  {hg[1]}       {hg[2]:9} {hr[2]:9} {hg[2] - hr[2]:5}  {'=' if hg[3] == hr[3] else 'X'}")
            print(f">> {g[0]:5}  {g[1]}/{r[1]}  {g[2]:9} {r[2]:9} {g[2] - r[2]:5}  "
                  f"{'=' if g[3] == r[3] else 'X ' + g[3] + '/' + r[3]}")
            return 1
        hist.append((g, r))
        if len(hist) > 4 * a.context:
            hist = hist[-a.context:]
    print(f"all {n} Z80 rows equal")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
