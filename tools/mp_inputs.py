"""Phase 7's stage inputs (xmen2disasm docs/phases/P7-multiplayer.md "Definitions"): game/mp_inputs/
boot.input (from power-on) and scene<N>.input for each stage start of tools/ws_warp.py --all, four pad
columns each, judged by what they reach (M4: every player in play for most of the gameplay frames).

    uv run tools/mp_inputs.py [--only boot,1,8] [--play N] [--out game/mp_inputs] [--check]

How one is made (the game decides the timing, so it is measured, not guessed): a probe input presses
each pad's Start in turn (pads 25 frames apart, a round every 100 frames) from frame 300; the
four-player runner (`--options players=4 --session 1`, `XMEN2_MP_LOG=1`) logs each change of scene,
level loop and players in play. The presses that count are those up to the last loop change before
all four players are in play (the briefing's pages, the select's joins and confirms; for boot.input
the SiberiaB1 opening, where players join with Start); later ones would pause the game. The input is
those presses, then from the frame all four are in, --play frames of each pad walking right with the
attacks and jumps of ws_warp.py's walker (or one of two other patterns), each pad on its own phase or
all four in step; the one kept is the one that meets M4's shares, else whose worst-off player does best
(and on a stage where none does, the longest of a few shorter plays that does: SHORTER). --check runs
the inputs written
and prints each one's `mp` counters.
"""
from __future__ import annotations

import argparse
import concurrent.futures
import itertools
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402
import ws_warp  # noqa: E402

PROBE_FRAMES = 12000
FIRST_PRESS = 300
WALK = [("8", 40), ("18", 4), ("8", 40), ("28", 4), ("8", 40), ("48", 4), ("8", 40), ("18", 6), ("8", 40),
        ("8", 72)]   # the walker's cycle (ws_warp.py) without its Start pair: right, B, C, A
# other ways to play, each tried; the input keeps the one whose worst-off player is in play longest
FIGHT = [("10", 4), ("0", 6), ("10", 4), ("0", 6), ("8", 12), ("10", 4), ("0", 6), ("20", 4), ("0", 10),
         ("40", 4), ("0", 6), ("10", 4), ("0", 6)]   # mostly attacks where they stand, a step right now and then
CAREFUL = [("8", 20), ("10", 4), ("0", 4), ("10", 4), ("0", 4), ("10", 4), ("0", 20), ("28", 6), ("8", 14),
           ("10", 4), ("0", 30)]   # a few steps, three attacks, a wait, a jump forward
PATTERNS = {"walk": WALK, "fight": FIGHT, "careful": CAREFUL}
PHASES = (37, 0)   # frames between pads in the pattern: each its own, or all four together (one group)
LOG = re.compile(r"^\[mp\] frame (\d+) scene (-?\d+) loop (\d) alive ([0-9a-f]+)", re.M)


def exe() -> Path:
    p = common.BUILD / "Release" / ("XMen2Native.exe" if sys.platform == "win32" else "XMen2Native")
    return p if p.is_file() else common.BUILD / "XMen2Native"


def write_input(path: Path, frames: list[list[int]]) -> None:
    lines, prev, run = ["# tools/mp_inputs.py (four pads: p1 p2 p3 p4)"], None, 0
    for cur in map(tuple, frames):
        if cur != prev and prev is not None:
            lines += ["PADS " + " ".join(f"{v:x}" for v in prev), f"WAIT {run}"]
            run = 0
        prev, run = cur, run + 1
    lines += ["PADS " + " ".join(f"{v:x}" for v in prev), f"WAIT {run}"]
    path.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")


def presses(until: int, order=(0, 1, 2, 3)) -> list[list[int]]:
    """frames 1..until: each pad's Start in turn (frame index 0 = runner frame 1). boot.input presses
    pads 3 and 4 before 2: SiberiaB1's opening, where players join with Start, ends when player 2 joins
    (SiberiaB1_FrameHook), as in the original."""
    f = [[0, 0, 0, 0] for _ in range(until)]
    for base in range(FIRST_PRESS, until, 100):
        for k, p in enumerate(order):
            for i in range(base + 25 * k, min(until, base + 25 * k + 4)):
                f[i - 1][p] |= 0x80
    return f


BOOT_ORDER = (0, 2, 3, 1)


def run(inp: Path, scene: int | None, frames: int, log: bool) -> tuple[int, str]:
    env = {**os.environ, "SDL_AUDIODRIVER": "dummy"}
    if log:
        env["XMEN2_MP_LOG"] = "1"
    cmd = [str(exe()), str(common.ROM), "--benchmark", str(frames), "--input-script", str(inp),
           "--session", "1", "--options", "players=4"]
    if scene is not None:
        cmd += ["--scene", str(scene)]
    r = subprocess.run(cmd, capture_output=True, text=True, env=env, timeout=3600)
    return r.returncode, r.stderr


def make(name: str, scene: int | None, play: int, out: Path) -> str:
    with tempfile.TemporaryDirectory(prefix="mp_inputs_") as td:
        probe = Path(td) / "probe.input"
        order = BOOT_ORDER if scene is None else (0, 1, 2, 3)
        write_input(probe, presses(PROBE_FRAMES, order))
        rc, err = run(probe, scene, PROBE_FRAMES, True)
    if rc:
        return f"{name}: the probe failed ({rc}): {err.strip()[-200:]}"
    ev = [(int(f), int(s), int(lp), int(a, 16)) for f, s, lp, a in LOG.findall(err)]
    allin = next((f for f, _s, _l, a in ev if a == 0xF), None)
    if allin is None:
        return f"{name}: the four players never came in ({len(ev)} events; last {ev[-1] if ev else None})"
    # the presses that count: up to the select's end, the last time a frame loop ended (loop 1 -> 0)
    # before all four were in (boot: the last join, the frame all four are in)
    ends = [f for (f, _s, lp, _a), (_f0, _s0, lp0, _a0) in zip(ev[1:], ev) if lp == 0 and lp0 == 1 and f <= allin]
    cut = allin + 4 if scene is None or not ends else ends[-1]   # boot: the last join's press whole
    head = presses(cut, order) + [[0, 0, 0, 0] for _ in range(allin - cut)]
    path = out / ("boot.input" if scene is None else f"scene{scene}.input")
    best = None
    for length in [play] + [n for n in SHORTER if n < play]:
        for (pname, pat), step in itertools.product(PATTERNS.items(), PHASES):
            seq = [int(m, 16) for m, n in pat for _ in range(n)]
            frames = head + [[seq[(i + step * p) % len(seq)] for p in range(4)] for i in range(length)]
            write_input(path, frames)   # each pad on its own phase of the pattern (0: all in step)
            mp = counters(path, scene)
            if not mp:
                continue
            gp = mp["gameplay_frames"] or 1
            worst = min(min(mp["in_play"][p], mp["drawn"][p] * 2) - 100 * mp["off_view"][p] for p in range(4)) / gp
            if best is None or (meets(mp), worst) > (meets(best[3]), best[0]):
                best = (worst, f"{pname}/{step}, {length} frames of play", frames, mp)
        if meets(best[3]) if best else False:
            break
    if best is None:
        return f"{name}: no pattern ran"
    write_input(path, best[2])
    mp = best[3]
    ip = ", ".join(f"{v / (mp['gameplay_frames'] or 1):.0%}" for v in mp["in_play"])
    return (f"{name}: {path.name} written ({best[1]}{'' if meets(mp) else '; short of M4'}): presses to {cut},"
            f" all four in at {allin}, {len(best[2])} frames; in play {ip}; off view {mp['off_view']}")


# A player who dies stays out until the stage ends (Player1/2_StartLife bring one back on Start only in
# SiberiaB1's opening), so on a stage whose enemies kill players early the full --play can't keep all four
# in play: then the longest of these shorter plays that does (M4's shares, as phase.py measures them)
SHORTER = (2000, 1400, 900)


def meets(mp: dict) -> bool:
    gp = mp["gameplay_frames"]
    return bool(gp) and mp["hud_missing"] == 0 and mp["sprites_dropped"] == 0 and all(
        mp["in_play"][p] >= 0.5 * gp and mp["drawn"][p] >= 0.5 * mp["in_play"][p]
        and mp["off_view"][p] <= 0.01 * mp["in_play"][p] for p in range(4))


def counters(path: Path, scene: int | None) -> dict | None:
    n = sum(int(m.group(1)) for m in re.finditer(r"^WAIT (\d+)", path.read_text(), re.M))
    rc, err = run(path, scene, n, False)
    m = re.search(r"\[xmen2\] counters (\{.*\})", err)
    return json.loads(m.group(1)).get("mp") if m and rc == 0 else None


def check(name: str, scene: int | None, out: Path) -> str:
    path = out / ("boot.input" if scene is None else f"scene{scene}.input")
    if not path.is_file():
        return f"{name}: no {path.name}"
    n = sum(int(m.group(1)) for m in re.finditer(r"^WAIT (\d+)", path.read_text(), re.M))
    rc, err = run(path, scene, n, False)
    m = re.search(r"\[xmen2\] counters (\{.*\})", err)
    mp = json.loads(m.group(1)).get("mp") if m else None
    return f"{name}: exit {rc} " + (json.dumps({k: mp[k] for k in ("gameplay_frames", "in_play", "drawn", "off_view",
                                                                  "hud_missing", "sprites_dropped")}) if mp else err[-200:])


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--only", default="", help="boot and/or scene numbers, comma-separated")
    ap.add_argument("--play", type=int, default=3000, help="frames of play after all four are in")
    ap.add_argument("--out", default=str(common.ROOT / "game" / "mp_inputs"))
    ap.add_argument("--check", action="store_true", help="run the inputs written, print their mp counters")
    a = ap.parse_args()
    common.ensure_rom()
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    items: list[tuple[str, int | None]] = [("boot", None)] + [
        (w, n) for w, n in sorted({**ws_warp.WORLDS, **ws_warp.LATER}.items(), key=lambda kv: kv[1])]
    if a.only:
        want = set(a.only.split(","))
        items = [(w, n) for w, n in items if (w if n is None else str(n)) in want or w in want]
    fn = (lambda it: check(it[0], it[1], out)) if a.check else (lambda it: make(it[0], it[1], a.play, out))
    with concurrent.futures.ThreadPoolExecutor(min(12, len(items))) as ex:
        for line in ex.map(fn, items):
            print(line, flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
