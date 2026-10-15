"""Dump a replay log (phase 4: build/.../replay.bin, engine exact_sched.c "Logical positions").

    uv run tools/replaydump.py DIR/replay.bin [--from N] [--count 40] [--sync S]

Records are numbered from 1 as the stepped runner counts them in its divergence messages
(a run of identical reads is one record). --sync S starts at the record of sync point S.
"""
from __future__ import annotations

import argparse
import struct


def records(path: str):
    with open(path, "rb") as f:
        data = f.read()
    assert data[:4] == b"X2RP", "not a replay log"
    p, n = 12, 0
    while p < len(data):
        t = chr(data[p])
        n += 1
        if t == "R":
            size, run, addr, val = struct.unpack_from("<BHIH", data, p + 1)
            yield n, f"R {addr:06X}.{size} = {val:04X} x{run}", None
            p += 10
        elif t == "I":
            level, depth, _z, reads, entries, since, until = struct.unpack_from("<BBBIQII", data, p + 1)
            yield n, f"I level {level} at depth {depth} entries {entries} reads {reads} +{since}/-{until} insns", None
            p += 24
        elif t == "S":
            _a, _b, frame, sync, entries = struct.unpack_from("<BHIQQ", data, p + 1)
            yield n, f"S sync {sync} frame {frame} entries {entries}", sync
            p += 24
        elif t == "X":
            depth, _z, reads, entries = struct.unpack_from("<BHIQ", data, p + 1)
            yield n, f"X end depth {depth} entries {entries} reads {reads}", None
            p += 16
        else:
            yield n, f"? byte {data[p]:02X} at offset {p}", None
            return


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("log")
    ap.add_argument("--from", dest="start", type=int, default=1)
    ap.add_argument("--count", type=int, default=40)
    ap.add_argument("--sync", type=int, default=0)
    a = ap.parse_args()
    shown, on = 0, False
    for n, text, sync in records(a.log):
        if not on and ((a.sync and sync == a.sync) or (not a.sync and n >= a.start)):
            on = True
        if on:
            print(f"{n:9d} {text}")
            shown += 1
            if shown >= a.count:
                break
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
