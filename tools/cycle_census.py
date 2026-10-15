"""Compare the recompiler's static per-instruction cycle costs with gpgx 2.4's base table.

    uv run tools/cycle_census.py [--gpgx PATH/m68ki_cycles.h] [--show 40]

Runs GenesisRecomp with GENESIS_CYCLE_DIAG (one row per emitted instruction: addr, opcode,
mnemonic, size, cost) and compares `cost` with gpgx's m68ki_cycles[opcode] / 7 (the reference
emulator's base cost, before its data-dependent extras). Prints mismatches grouped by mnemonic
and opcode. The gpgx table is read from xmen2disasm's BizHawk 2.4 source checkout
(emu/_src/bizhawk-2.4, fetched by its emu/build_core.py); it is never copied here.
"""
from __future__ import annotations

import argparse
import collections
import csv
import os
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

DEFAULT_TABLE = (common.ROOT.parent / "xmen2disasm" / "emu" / "_src" / "bizhawk-2.4" / "waterbox" / "gpgx"
                 / "core" / "m68k" / "m68ki_cycles.h")


def gpgx_table(path: Path) -> list[int]:
    text = path.read_text()
    body = text[text.index("m68ki_cycles[0x10000]"):]
    vals = [int(m.group(1)) for m in re.finditer(r"(\d+)\*7", body)]
    if len(vals) < 0x10000:
        sys.exit(f"parsed only {len(vals)} entries from {path}")
    return vals[:0x10000]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--gpgx", default=str(DEFAULT_TABLE))
    ap.add_argument("--show", type=int, default=60)
    ap.add_argument("--census", help="reuse an existing census CSV instead of running the recompiler")
    a = ap.parse_args()
    table = gpgx_table(Path(a.gpgx))
    census = Path(a.census) if a.census else common.BUILD / "cycle_census.csv"
    if not a.census:
        rec = next(p for p in common.BUILD.rglob("GenesisRecomp.exe") if p.is_file())
        out = common.BUILD / "census-gen"
        out.mkdir(parents=True, exist_ok=True)
        env = {**os.environ, "GENESIS_CYCLE_DIAG": str(census)}
        r = subprocess.run([str(rec), "xmen2.bin", "--game", "game.toml", "--output-dir", str(out)],
                           cwd=common.GAME, env=env, capture_output=True, text=True)
        if r.returncode:
            sys.exit(r.stdout[-2000:] + r.stderr[-2000:])
    rows = {}
    with open(census, newline="") as f:
        for r in csv.DictReader(f):
            rows[int(r["addr"], 16)] = (int(r["opcode"], 16), int(r["mnemonic"]), int(r["cost"]))
    bad = collections.defaultdict(list)
    for addr, (op, mn, cost) in sorted(rows.items()):
        ref = table[op]
        if ref != cost:
            bad[(mn, op, cost, ref)].append(addr)
    print(f"{len(rows)} instructions, {sum(len(v) for v in bad.values())} with a different base cost "
          f"({len(bad)} distinct opcode/cost pairs)")
    by_mn = collections.Counter()
    for (mn, op, cost, ref), addrs in bad.items():
        by_mn[mn] += len(addrs)
    print("by mnemonic id:", dict(by_mn.most_common()))
    for (mn, op, cost, ref), addrs in sorted(bad.items(), key=lambda kv: -len(kv[1]))[:a.show]:
        print(f"  mn {mn:3} op {op:04X}: ours {cost:3} gpgx {ref:3}  x{len(addrs):5}  e.g. ${addrs[0]:06X}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
