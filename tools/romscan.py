"""Does a file contain ROM data? (xmen2disasm docs/phases/P5-port.md P8/P9: the package holds no
ROM-derived data: no 256-byte window of any packaged file, other than runs of one byte value,
occurs in the ROM.)

    uv run tools/romscan.py FILE... [--rom game/xmen2.bin] [--json]

Method. Any 256-byte string shared by a file and the ROM contains at least three whole 64-byte
blocks of the file at offsets divisible by 64. So every 64-byte window of the ROM is indexed once
by a rolling hash, each aligned 64-byte block of the file that is not one byte value is looked up,
and a hit is verified against the ROM bytes and extended both ways; it is a ROM window when the
common stretch is 256 bytes or longer and not a run of one byte value. (A shared 256-byte window
made only of whole one-value blocks, e.g. 128 zeros then 128 $FF, is not searched: it is runs.)
Reports the windows found (count of maximal common stretches, with offsets).
"""
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

B = 64           # block size (a 256-byte window holds >= 3 aligned blocks)
WINDOW = 256
MOD = (1 << 61) - 1
BASE = 257


def block_hash(b: bytes) -> int:
    h = 0
    for x in b:
        h = (h * BASE + x) % MOD
    return h


def rom_index(rom: bytes) -> set[int]:
    """Rolling hashes of every 64-byte window of the ROM."""
    top = pow(BASE, B - 1, MOD)
    h = block_hash(rom[:B])
    out = {h}
    for i in range(B, len(rom)):
        h = ((h - rom[i - B] * top) * BASE + rom[i]) % MOD
        out.add(h)
    return out


def scan(data: bytes, rom: bytes, index: set[int]) -> list[dict]:
    found: dict[tuple[int, int], int] = {}   # (file start, rom start) -> length
    for a in range(0, len(data) - B + 1, B):
        blk = data[a:a + B]
        if blk.count(blk[0]) == B or block_hash(blk) not in index:
            continue
        r = rom.find(blk)
        while r >= 0:
            lo = 0
            while a - lo > 0 and r - lo > 0 and data[a - lo - 1] == rom[r - lo - 1]:
                lo += 1
            hi = B
            while a + hi < len(data) and r + hi < len(rom) and data[a + hi] == rom[r + hi]:
                hi += 1
            start, n = a - lo, lo + hi
            seg = data[start:start + n]
            if n >= WINDOW and seg.count(seg[0]) != n:
                # a run of one value longer than the window is allowed; a mixed stretch is not
                found[(start, r - lo)] = n
            r = rom.find(blk, r + 1)
    return [{"file_offset": f, "rom_offset": r, "bytes": n} for (f, r), n in sorted(found.items())]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="+", type=Path)
    ap.add_argument("--rom", type=Path, default=common.ROM)
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args()
    rom = a.rom.read_bytes()
    index = rom_index(rom)
    res = {str(f): scan(f.read_bytes(), rom, index) for f in a.files}
    total = sum(len(v) for v in res.values())
    if a.json:
        print(json.dumps({"rom_windows": total, "files": res}))
    else:
        for f, v in res.items():
            print(f"{f}: {len(v)} ROM windows" + (f" (first at file +${v[0]['file_offset']:X} = ROM "
                                                   f"${v[0]['rom_offset']:06X}, {v[0]['bytes']} bytes)" if v else ""))
    return 1 if total else 0


if __name__ == "__main__":
    raise SystemExit(main())
