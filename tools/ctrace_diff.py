"""Find the first instruction where two per-instruction cycle traces disagree.

    uv run tools/ctrace_diff.py GAME.csv REF.csv [--context 12]

Both files are `frame,pc,mclk,sr,a7[,rh]` (game: GENESIS_XTRACE=from,to,path on the runner in
exact mode; reference: xmen2disasm `uv run emu/cycle_trace.py RUN --from A --to B --out F`).
rh is a hash of D0-D7/A0-A7 at the instruction's start. Rows are compared in order: pc, mclk,
sr, a7, rh. The report shows the rows before the first difference: a register-hash change
means the previous instruction computed a different value; an mclk change, a different cost.
"""
from __future__ import annotations

import argparse
import itertools


def rows(path):
    with open(path) as f:
        next(f)
        for line in f:
            p = line.rstrip("\n").split(",")
            yield (int(p[0]), p[1], int(p[2]), p[3].upper(), p[4].upper(), p[5].upper() if len(p) > 5 else "")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("game")
    ap.add_argument("ref")
    ap.add_argument("--context", type=int, default=12)
    a = ap.parse_args()
    hist = []
    n = 0
    for g, r in itertools.zip_longest(rows(a.game), rows(a.ref)):
        n += 1
        if g is None or r is None:
            print(f"trace ends after {n - 1} matching rows ({'game' if g is None else 'ref'} shorter)")
            return 0 if g is None and r is None else 1
        same = g[1:5] == r[1:5] and (not g[5] or not r[5] or g[5] == r[5])
        if not same:
            print(f"first difference at row {n}:")
            print("   frame  pc      game_mclk ref_mclk  dclk  sr         a7             regs")
            for hg, hr in hist[-a.context:]:
                print(f"   {hg[0]:5}  {hg[1]}  {hg[2]:9} {hr[2]:9} {hg[2] - hr[2]:5}  {hg[3]}/{hr[3]}  {hg[4]}/{hr[4]}  "
                      f"{'=' if hg[5] == hr[5] else 'X'}")
            print(f">> {g[0]:5}  {g[1]}/{r[1]}  {g[2]:9} {r[2]:9} {g[2] - r[2]:5}  {g[3]}/{r[3]}  {g[4]}/{r[4]}  "
                  f"{'=' if g[5] == r[5] else 'X ' + g[5] + '/' + r[5]}")
            return 1
        hist.append((g, r))
        if len(hist) > 4 * a.context:
            hist = hist[-a.context:]
    print(f"all {n} rows equal")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
