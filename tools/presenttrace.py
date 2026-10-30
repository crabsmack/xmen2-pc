"""What the display showed, from a played run's XMEN2_PRESENT_TRACE (host_sdl.cpp; NOTES "Present
trace"): the frame-pacing measure.

    XMEN2_PRESENT_TRACE=T.csv XMen2Native.exe ROM [--options high_fps=120] ...
    uv run tools/presenttrace.py T.csv [--json] [--skip-s 2]

Each picture is taken as shown from the first refresh after SDL_RenderPresent returned (the V-blank
grid from DWM's qpcVBlank and qpcRefreshPeriod, Windows only), and a later picture before that
refresh replaces it (dropped). Per picture that reaches the screen: how many refreshes it stayed
against the content time to the next one; `judder` counts the pictures off by over half a refresh
(smooth motion keeps each picture up for as long as the game time it covers, at any rate, so 60 on a
240 Hz display, 4 refreshes each, is smooth, and 180 on 240 is not). `late` counts presents that started over 1 ms after they were due (the timer),
and `present_ms` is how long SDL_RenderPresent took (a blocking present waits for the display).
A constant latency between the present and the refresh doesn't change any of these.
"""
from __future__ import annotations

import argparse
import collections
import csv
import json
import statistics
import sys
from pathlib import Path


def analyse(path: Path, skip_s: float, display_hz: float = 0.0) -> dict:
    lines = path.read_text().splitlines()
    head = dict(zip(lines[0].split()[1::2], lines[0].split()[2::2])) if lines and lines[0].startswith("#") else {}
    freq = float(head.get("freq", 1e7))
    rows = list(csv.DictReader(l for l in lines if not l.startswith("#")))
    if not rows:
        return {"ok": False, "detail": "no rows"}
    p0 = int(rows[0]["pre"])
    rows = [r for r in rows if (int(r["pre"]) - p0) / freq >= skip_s]   # the start-up's frames
    out: dict = {"ok": True, "trace": str(path), "header": head, "presents": len(rows),
                 "seconds": round((int(rows[-1]["post"]) - int(rows[0]["pre"])) / freq, 2) if rows else 0}
    if len(rows) < 3:
        return {**out, "ok": False, "detail": "too few rows"}
    out["presents_per_s"] = round(len(rows) / out["seconds"], 1)
    pre = [int(r["pre"]) for r in rows]
    post = [int(r["post"]) for r in rows]
    due = [int(r["due"]) for r in rows]
    late = [(p - d) / freq * 1e3 for p, d in zip(pre, due) if d]
    out["late"] = sum(x > 1.0 for x in late)
    out["late_ms_max"] = round(max(late), 2) if late else None
    took = sorted((b - a) / freq * 1e3 for a, b in zip(pre, post))
    out["present_ms"] = {"p50": round(took[len(took) // 2], 3), "p99": round(took[int(len(took) * 0.99)], 3),
                         "max": round(took[-1], 3)}
    periods = [int(r["period"]) for r in rows if int(r["period"])]
    if display_hz:   # --display-hz: these present times against another display's refreshes (an estimate)
        for r in rows:
            r["vblank"], r["period"] = "0", "0"
        period, out["simulated_display"] = freq / display_hz, True
    elif not periods:
        return {**out, "detail": "no V-blank times (not Windows, or DWM gave none): timing only"}
    else:
        period = statistics.median(periods)
    out["refresh_hz"] = round(freq / period, 3)
    v0 = int(rows[0]["vblank"]) or post[0]
    shown_at: dict[int, float] = {}   # refresh index -> the content time of the last picture before it
    dropped = 0
    for r, b in zip(rows, post):
        vb = int(r["vblank"]) or v0
        while vb - period > b:
            vb -= period
        while vb <= b:
            vb += period
        i = round((vb - v0) / period)
        dropped += i in shown_at
        shown_at[i] = float(r["content_us"])
    out["dropped"] = dropped
    first, last = min(shown_at), max(shown_at)
    seq, cur = [], None
    for i in range(first, last + 1):
        cur = shown_at.get(i, cur)
        seq.append(cur)
    p_us = period / freq * 1e6
    out["refreshes"] = len(seq)
    out["repeats"] = sum(a == b for a, b in zip(seq, seq[1:]))
    # each new picture: how long it stayed on screen (refreshes until the next one) against the
    # content time to the next one; judder = off by over half a refresh
    changes = [i for i in range(1, len(seq)) if seq[i] != seq[i - 1]]
    held, judder = collections.Counter(), 0
    for a, b in zip(changes, changes[1:]):
        held[b - a] += 1
        judder += abs((b - a) * p_us - (seq[b] - seq[a])) > 0.5 * p_us
    out["pictures"] = len(changes)
    out["judder"] = judder
    out["judder_pct"] = round(100 * judder / max(1, len(changes) - 1), 2)
    out["refreshes_per_picture"] = dict(sorted(held.items()))
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("trace", nargs="+")
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--skip-s", type=float, default=2.0, help="leave out the first seconds (start-up)")
    ap.add_argument("--display-hz", type=float, default=0.0,
                    help="the present times against a display at this rate instead of the measured one (an estimate)")
    a = ap.parse_args()
    res = [analyse(Path(t), a.skip_s, a.display_hz) for t in a.trace]
    if a.json:
        print(json.dumps(res if len(res) > 1 else res[0], indent=1))
        return 0
    for r in res:
        if not r.get("refreshes"):
            print(f"{r.get('trace')}: {r.get('detail')} {r}")
            continue
        h = r["header"]
        print(f"{Path(r['trace']).name}: high_fps {h.get('high_fps')}, display {r['refresh_hz']} Hz, {h.get('renderer')}; "
              f"{r['presents_per_s']} presents/s, {r['dropped']} dropped; {r['pictures']} pictures over {r['refreshes']} "
              f"refreshes, judder {r['judder_pct']}%, refreshes per picture {r['refreshes_per_picture']}; "
              f"late {r['late']} (max {r['late_ms_max']} ms); present ms {r['present_ms']}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
