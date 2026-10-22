"""Phase 6 development test (beyond the corpus, which reaches only Siberia and the attract demos in
the native machine): start a new game at each world's Cerebro briefing (XMEN2_DEBUG_SCENE), play a
'walker' input (right held, B/C/A presses, a Start pair every 300 frames: pause and unpause in a
stage, the next page on a Cerebro screen) headless, and report the runner's `ws` counters and
screens per world, with screenshots every 1,500 frames. --game-state (R8, presentation only): each
world also runs with widescreen=0, both with --game-rows, and the two must be equal.

    uv run tools/ws_warp.py OUT [--frames 30000] [--options widescreen=16:9,...] [--all] [--game-state]
"""
from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

WORLDS = {"sentinels": 1, "avalon": 8, "banimaza": 18, "savage": 24, "space": 34, "clone": 38}
# --all: also the player select before each later stage (Scene_RegisterAll's order), and the two
# stages that follow a stage directly (SentinelsB2 after SentinelsA1: scene 7)
LATER = {"sentinelsB1": 3, "sentinelsA1": 5, "sentinelsB2": 7, "avalonB2": 10, "avalonA2": 12, "avalonA3": 14,
         "avalonA4": 16, "banimazaA1": 20, "banimazaA2": 22, "savageB1": 26, "savageA2": 28, "savageA3": 30,
         "savageA4": 32, "spaceA1": 36, "cloneA2": 40, "cloneA5": 42}
CYCLE = [("8", 40), ("18", 4), ("8", 40), ("28", 4), ("8", 40), ("48", 4), ("8", 40), ("18", 6), ("8", 40),
         ("80", 2), ("0", 6), ("80", 2), ("8", 72)]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("--frames", type=int, default=30000)
    ap.add_argument("--options", default="widescreen=16:9")
    ap.add_argument("--all", action="store_true", help="every stage, not only each world's first")
    ap.add_argument("--game-state", action="store_true",
                    help="R8: also run widescreen=0 and compare the game's state (--game-rows) with it")
    a = ap.parse_args()
    scenes = {**WORLDS, **(LATER if a.all else {})}
    common.ensure_rom()
    exe = common.BUILD / "Release" / ("XMen2Native.exe" if sys.platform == "win32" else "XMen2Native")
    if not exe.is_file():
        exe = common.BUILD / "XMen2Native"
    out = Path(a.out).resolve()
    out.mkdir(parents=True, exist_ok=True)
    lines, n = ["# walker (tools/ws_warp.py)"], 0
    while n < a.frames + 400:
        for pad, k in CYCLE:
            lines += [f"PADS {pad} 0", f"WAIT {k}"]
            n += k
    (out / "walker.txt").write_text("\n".join(lines) + "\n")

    def play(world: str, d: Path, options: str, shots: bool):
        d.mkdir(exist_ok=True)
        env = {**os.environ, "XMEN2_DEBUG_SCENE": str(scenes[world]), "SDL_AUDIODRIVER": "dummy"}
        if shots:
            env.update(XMEN2_SHOTS=",".join(str(f) for f in range(1500, a.frames + 1, 1500)), XMEN2_DUMP_DIR=str(d))
        r = subprocess.run([str(exe), str(common.ROM), "--benchmark", str(a.frames), "--input-script",
                            str(out / "walker.txt"), "--options", options,
                            *(["--game-rows", str(d / "game.csv")] if a.game_state else [])],
                           capture_output=True, text=True, env=env)
        (d / "stderr.txt").write_text(r.stderr)
        m = re.search(r"\[xmen2\] counters (\{.*\})", r.stderr)
        return (json.loads(m.group(1))["enhance"] if m and r.returncode == 0 else None), r.stderr[-300:]

    def one(world: str):
        e, err = play(world, out / world, a.options, True)
        if e is not None and a.game_state:
            off, err0 = play(world, out / world / "off", "widescreen=0", False)
            if off is None:
                return world, None, "widescreen=0: " + err0
            ga = (out / world / "off" / "game.csv").read_text().splitlines()
            gb = (out / world / "game.csv").read_text().splitlines()
            k = next((i for i, (x, y) in enumerate(zip(ga, gb)) if x != y), None if len(ga) == len(gb) else min(len(ga), len(gb)))
            e["game_state"] = "equal" if k is None else f"differs from widescreen=0 at row {k} of {len(ga) - 1}"
        return world, e, err

    bad = 0
    with ThreadPoolExecutor(len(scenes)) as ex:
        for world, e, err in ex.map(one, scenes):
            if e is None:
                bad += 1
                print(f"{world}: FAILED {err}")
                continue
            ws = dict(e.get("ws") or {})
            screens = ws.pop("screens", {})
            bad += any(ws.get(k) for k in ("terrain_errors", "sprites_lost", "hud_misplaced", "bars_unneeded"))
            bad += e.get("game_state", "equal") != "equal"
            print(f"{world}: {json.dumps(ws)}" + (f"\n   game state: {e['game_state']}" if "game_state" in e else "")
                  + f"\n   screens: {', '.join(f'{k} ({v})' for k, v in screens.items())}")
    return 1 if bad else 0


if __name__ == "__main__":
    raise SystemExit(main())
