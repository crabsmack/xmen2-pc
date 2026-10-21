"""Phase 6 development test (beyond the corpus, which reaches only Siberia and the attract demos in
the native machine): start a new game at each world's Cerebro briefing (XMEN2_DEBUG_SCENE), play a
'walker' input (right held, B/C/A presses, a Start pair every 300 frames: pause and unpause in a
stage, the next page on a Cerebro screen) headless, and report the runner's `ws` counters and
screens per world, with screenshots every 1,500 frames.

    uv run tools/ws_warp.py OUT [--frames 30000] [--options widescreen=16:9,...]
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
CYCLE = [("8", 40), ("18", 4), ("8", 40), ("28", 4), ("8", 40), ("48", 4), ("8", 40), ("18", 6), ("8", 40),
         ("80", 2), ("0", 6), ("80", 2), ("8", 72)]


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("--frames", type=int, default=30000)
    ap.add_argument("--options", default="widescreen=16:9")
    a = ap.parse_args()
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

    def one(world: str):
        d = out / world
        d.mkdir(exist_ok=True)
        env = {**os.environ, "XMEN2_DEBUG_SCENE": str(WORLDS[world]), "SDL_AUDIODRIVER": "dummy",
               "XMEN2_SHOTS": ",".join(str(f) for f in range(1500, a.frames + 1, 1500)), "XMEN2_DUMP_DIR": str(d)}
        r = subprocess.run([str(exe), str(common.ROM), "--benchmark", str(a.frames), "--input-script",
                            str(out / "walker.txt"), "--options", a.options], capture_output=True, text=True, env=env)
        (d / "stderr.txt").write_text(r.stderr)
        m = re.search(r"\[xmen2\] counters (\{.*\})", r.stderr)
        return world, (json.loads(m.group(1))["enhance"] if m and r.returncode == 0 else None), r.stderr[-300:]

    bad = 0
    with ThreadPoolExecutor(len(WORLDS)) as ex:
        for world, e, err in ex.map(one, WORLDS):
            if e is None:
                bad += 1
                print(f"{world}: FAILED {err}")
                continue
            ws = dict(e.get("ws") or {})
            screens = ws.pop("screens", {})
            bad += any(ws.get(k) for k in ("terrain_errors", "sprites_lost", "hud_misplaced"))
            print(f"{world}: {json.dumps(ws)}\n   screens: {', '.join(f'{k} ({v})' for k, v in screens.items())}")
    return 1 if bad else 0


if __name__ == "__main__":
    raise SystemExit(main())
