"""The launcher (platform/native/launcher.cpp, GitHub issue #1), headless: each case copies the native
runner into its own folder (so its xmen2.ini is the case's), drives the launcher with a test script
(XMEN2_LAUNCHER_TEST, SDL's dummy video driver) and checks what it hands the game (XMEN2_LAUNCHER_DRYRUN
prints the plan instead of playing) and what it writes to xmen2.ini. Screenshots of its pages land in
OUT/shots for a person to look at.

    uv run tools/launcher_check.py OUT [--exe build/Release/XMen2Native.exe]

Cases: play (options set, saved, the plan's options), host 4 (hub, seat 0, players=4 set), join (lan, the
default port added to the address, seat 1's port), a wrong ROM (PLAY refused, nothing written), show=0 then
a start without the launcher (the game runs from [rom], no launcher), and --launcher overriding show=0.
"""
from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

EXE_NAME = "XMen2Native.exe" if sys.platform == "win32" else "XMen2Native"


def run_case(exe: Path, d: Path, script: str | None, args: list[str], dry: bool = True, timeout: int = 120):
    d.mkdir(parents=True, exist_ok=True)
    local = d / exe.name
    if not local.exists():
        shutil.copy2(exe, local)
        for dll in exe.parent.glob("*.dll"):
            shutil.copy2(dll, d / dll.name)
    env = {**os.environ, "SDL_VIDEODRIVER": "dummy", "SDL_AUDIODRIVER": "dummy"}
    env.pop("XMEN2_LAUNCHER_TEST", None)
    env.pop("XMEN2_LAUNCHER_DRYRUN", None)
    if script is not None:
        (d / "script.txt").write_text(script)
        env["XMEN2_LAUNCHER_TEST"] = str(d / "script.txt")
    if dry:
        env["XMEN2_LAUNCHER_DRYRUN"] = "1"
    r = subprocess.run([str(local), *args], capture_output=True, text=True, env=env, timeout=timeout, cwd=d)
    m = re.search(r"^\[launcher\] plan (\{.*\})$", r.stdout, re.M)
    plan = json.loads(m.group(1).replace("\\", "\\\\")) if m else None
    ini = (d / "xmen2.ini").read_text() if (d / "xmen2.ini").exists() else ""
    return r, plan, ini


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("--exe", default=str(common.BUILD / "Release" / EXE_NAME))
    a = ap.parse_args()
    common.ensure_rom()
    exe = Path(a.exe).resolve()
    out = Path(a.out).resolve()
    if out.exists():
        shutil.rmtree(out)
    shots = out / "shots"
    shots.mkdir(parents=True)
    rom = str(common.ROM)
    fails = []

    def check(name: str, ok: bool, why: str):
        print(f"{'PASS' if ok else 'FAIL'}  {name}: {why}")
        if not ok:
            fails.append(name)

    # 1. play: no ROM known -> chosen; options set; the plan and the file
    r, plan, ini = run_case(exe, out / "play", f"""shot {shots / '1_no_rom.bmp'}
rom {rom}
shot {shots / '2_play.bmp'}
set lives inf
set start_stage avalon_a4
set widescreen 16:9
tab options
focus start_stage
shot {shots / '3_options.bmp'}
tab play
shot {shots / '4_play_changed.bmp'}
press play
""", [])
    ok = plan is not None and plan["rom"].replace("\\", "/") == rom.replace("\\", "/") and plan["netplay"] == [] \
        and plan["options"].get("lives") == "inf" and plan["options"].get("start_stage") == "avalon_a4"
    ok = ok and "lives = inf" in ini and "start_stage = avalon_a4" in ini and "widescreen = 16:9" in ini \
        and "[rom]" in ini and "show = 1" in ini and "[netplay]" not in ini
    check("play", ok, f"plan {plan}, ini {ini.count(chr(10))} lines" + ("" if ok else f"\n{r.stderr[-600:]}\n{ini}"))

    # 2. host four players: hub, seat 0, its port, players=4 into the options
    r, plan, ini = run_case(exe, out / "host", f"""rom {rom}
tab online
mode host
field players 4
shot {shots / '5_host.bmp'}
press host
""", [])
    want = ["--netplay", "hub", "--seat", "0", "--seats", "4", "--bind", "0.0.0.0:7000"]
    ok = plan is not None and plan["netplay"] == want and plan["options"].get("players") == "4" \
        and "mode = host" in ini and "players = 4" in ini
    check("host 4", ok, f"netplay {plan and plan['netplay']}, players {plan and plan['options'].get('players')}")

    # 3. join two players: lan, seat 1, its own port, the host's address with the default port added
    r, plan, ini = run_case(exe, out / "join", f"""rom {rom}
tab online
mode join
field players 2
field host 192.168.1.20
shot {shots / '6_join.bmp'}
press join
""", [])
    want = ["--netplay", "lan", "--seat", "1", "--seats", "2", "--bind", "0.0.0.0:7001", "--peer", "192.168.1.20:7000"]
    ok = plan is not None and plan["netplay"] == want and "mode = join" in ini and "host = 192.168.1.20" in ini
    check("join", ok, f"netplay {plan and plan['netplay']}")

    # 4. a wrong ROM: PLAY refused (no plan), the error shown, nothing written; then quit
    r, plan, ini = run_case(exe, out / "badrom", f"""rom {exe}
press play
shot {shots / '7_bad_rom.bmp'}
press quit
""", [])
    check("wrong ROM", plan is None and r.returncode == 0 and ini == "", f"exit {r.returncode}, plan {plan}, ini {ini!r}")

    # 5. show = 0, then a start without the launcher: the game plays from [rom] (5 frames, dummy video)
    r, plan, ini = run_case(exe, out / "noshow", f"""rom {rom}
show 0
press play
""", [])
    ok = plan is not None and "show = 0" in ini
    r2, plan2, _ = run_case(exe, out / "noshow", None, ["--quit-after", "5"], dry=False, timeout=300)
    ok2 = r2.returncode == 0 and plan2 is None and "[launcher]" not in r2.stdout + r2.stderr
    check("show = 0", ok and ok2, f"ini has show = 0: {'show = 0' in ini}; the next start ran the game without it: exit "
          f"{r2.returncode}" + ("" if ok2 else f"\n{r2.stderr[-600:]}"))

    # 6. --launcher shows it anyway (its script runs and presses play: a plan)
    r, plan, ini = run_case(exe, out / "noshow", "press play\n", ["--launcher"])
    check("--launcher", plan is not None, f"plan {plan is not None}")

    # 7. scripted and headless starts never see it: --benchmark with no ROM argument and no ini
    r, plan, ini = run_case(exe, out / "bench", "press quit\n", ["--benchmark", "3"], dry=True)
    check("--benchmark skips it", plan is None and "[launcher]" not in r.stderr, f"exit {r.returncode}")

    for f in shots.glob("*.bmp"):
        try:
            from PIL import Image
            Image.open(f).save(f.with_suffix(".png"))
            f.unlink()
        except ImportError:
            break
    print(f"{7 - len(fails)}/7 pass; screenshots in {shots}")
    return 1 if fails else 0


if __name__ == "__main__":
    raise SystemExit(main())
