"""Does a file contain ROM data? (xmen2disasm docs/phases/P5-port.md P8/P9: the package holds no
ROM-derived data: no 256-byte window of any packaged file, other than runs of one byte value,
occurs in the ROM. Amended 2026-11-24 (the user): a window that is one byte value but for at most
RUN_SLACK bytes counts as a run; librashader.dll's one $08 byte before 356 zeros, which the ROM
also has, carries a byte of information and nothing of the game.)

    uv run tools/romscan.py FILE... [--rom game/xmen2.bin] [--json]
    uv run tools/romscan.py --selftest [--rom game/xmen2.bin]   # the negative control: a planted ROM
        window is found, near-runs within the slack are not, one byte past it is

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
RUN_SLACK = 8    # a window with at most this many bytes other than its commonest value is a run
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


def only_runs(seg: bytes) -> bool:
    """Every 256-byte window of seg is a run: one byte value but for at most RUN_SLACK bytes."""
    counts = [0] * 256
    for x in seg[:WINDOW]:
        counts[x] += 1
    for i in range(len(seg) - WINDOW + 1):
        if i:
            counts[seg[i - 1]] -= 1
            counts[seg[i + WINDOW - 1]] += 1
        if WINDOW - max(counts) > RUN_SLACK:
            return False
    return True


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
            if n >= WINDOW and not only_runs(seg):
                # runs (one value, but for RUN_SLACK bytes a window) are allowed; a mixed stretch is not
                found[(start, r - lo)] = n
            r = rom.find(blk, r + 1)
    return [{"file_offset": f, "rom_offset": r, "bytes": n} for (f, r), n in sorted(found.items())]


def selftest(rom: bytes, index: set[int]) -> list[str]:
    """Planted cases in noise that matches nothing: what must be found and what must not; the failures."""
    import random
    rng = random.Random(1)
    fails = []
    # a near-run the ROM has: the longest stretch of zeros, with the byte before it
    z = max(((m.start(), m.end()) for m in __import__("re").finditer(rb"\x00{300,}", rom)), key=lambda t: t[1] - t[0])
    near = rom[z[0] - 1:z[0] + WINDOW + 40] if z[0] > 0 else None
    mixed = next(rom[o:o + WINDOW + 40] for o in range(0x10000, len(rom), 4096)
                 if not only_runs(rom[o:o + WINDOW + 40]))
    # the rule at its edge: RUN_SLACK other bytes in a window is a run, one more is not (spread out too)
    for k, want in ((RUN_SLACK, True), (RUN_SLACK + 1, False)):
        for spread in (1, 25):
            seg = bytearray(300)
            for i in range(k):
                seg[10 + i * spread] = 0x08
            ok = only_runs(bytes(seg)) == want
            print(f"selftest: {k} other bytes {spread} apart: {'a run' if want else 'not a run'} ({'ok' if ok else 'WRONG'})")
            if not ok:
                fails.append(f"{k} other bytes {spread} apart")
    cases = [("a mixed ROM window", mixed, True), ("the ROM's near-run (one byte, then zeros)", near, False)]
    for name, plant, want in cases:
        if plant is None:
            fails.append(f"{name}: none in this ROM to plant")
            continue
        data = bytearray(rng.getrandbits(8) for _ in range(8192))
        data[3000:3000 + len(plant)] = plant
        got = bool(scan(bytes(data), rom, index))
        print(f"selftest: {name}: {'found' if got else 'not found'} ({'ok' if got == want else 'WRONG'})")
        if got != want:
            fails.append(name)
    return fails


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="*", type=Path)
    ap.add_argument("--rom", type=Path, default=common.ROM)
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--selftest", action="store_true")
    a = ap.parse_args()
    rom = a.rom.read_bytes()
    index = rom_index(rom)
    if a.selftest:
        return 1 if selftest(rom, index) else 0
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
