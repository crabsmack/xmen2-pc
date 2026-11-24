"""The launcher (platform/native/launcher.cpp, GitHub issue #1), headless: each case copies the native
runner into its own folder (so its xmen2.ini is the case's), drives the launcher with a test script
(XMEN2_LAUNCHER_TEST, SDL's dummy video driver) and checks what it hands the game (XMEN2_LAUNCHER_DRYRUN
prints the plan instead of playing) and what it writes to xmen2.ini. Screenshots of its pages land in
OUT/shots for a person to look at.

    uv run tools/launcher_check.py OUT [--exe build/Release/XMen2Native.exe]

Cases: play (options set, saved, the plan's options), host 4 (hub, seat 0, players=4 set), join (lan, the
default port added to the address, seat 1's port), a wrong ROM (PLAY refused, nothing written), show=0 then
a start without the launcher (the game runs from [rom], no launcher), and --launcher overriding show=0,
the Controls page (a virtual controller's input caught, keyboard layouts, [playerN] written and read back),
the pad selftest (controllers' slots and bindings), join codes, and RecompLauncher's start (its [rom] path,
the program started bare).
"""
from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

EXE_NAME = "XMen2Native.exe" if sys.platform == "win32" else "XMen2Native"
# Every case, whatever its script: a join code's room asks no router (UPnP, NAT-PMP) and no STUN server, and
# binds loopback only (no firewall question on the desktop). A case that forgot `field connection direct`
# once asked this machine's real router to forward a port.
NO_ROUTER = {"RNET_HOST_RELAY_NO_ROUTER": "1", "XMEN2_ROOM_BIND": "127.0.0.1",
             "RNET_HOST_RELAY_ENDPOINT": "127.0.0.1:7000"}


def run_case(exe: Path, d: Path, script: str | None, args: list[str], dry: bool = True, timeout: int = 120):
    d.mkdir(parents=True, exist_ok=True)
    local = d / exe.name
    if not local.exists():
        shutil.copy2(exe, local)
        for dll in exe.parent.glob("*.dll"):
            shutil.copy2(dll, d / dll.name)
    env = {**os.environ, "SDL_VIDEODRIVER": "dummy", "SDL_AUDIODRIVER": "dummy", **NO_ROUTER}
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


def start_case(exe: Path, d: Path, script: str, extra_env: dict, dry: bool = True) -> subprocess.Popen:
    """run_case's launcher, started in the background (a room's host and joiners play side by side); dry=False
    goes on into the game the launcher plans"""
    d.mkdir(parents=True, exist_ok=True)
    local = d / exe.name
    if not local.exists():
        shutil.copy2(exe, local)
        for dll in exe.parent.glob("*.dll"):
            shutil.copy2(dll, d / dll.name)
    env = {**os.environ, "SDL_VIDEODRIVER": "dummy", "SDL_AUDIODRIVER": "dummy", "XMEN2_NO_MESSAGEBOX": "1",
           **({"XMEN2_LAUNCHER_DRYRUN": "1"} if dry else {}), **NO_ROUTER, **extra_env}
    (d / "script.txt").write_text(script)
    env["XMEN2_LAUNCHER_TEST"] = str(d / "script.txt")
    return subprocess.Popen([str(local)], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, env=env, cwd=d)


def finish_case(p: subprocess.Popen, d: Path, timeout: int = 90):
    """-> (result with .stdout/.stderr/.returncode, the plan or None, xmen2.ini)"""
    try:
        so, se = p.communicate(timeout=timeout)
    except subprocess.TimeoutExpired:
        p.kill()
        so, se = p.communicate()
    r = subprocess.CompletedProcess(p.args, p.returncode, so, se)
    m = re.search(r"^\[launcher\] plan (\{.*\})$", so, re.M)
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
    checks = [0]

    def check(name: str, ok: bool, why: str):
        checks[0] += 1
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
field connection direct
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
field connection direct
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

    # 7a. RecompLauncher (platform/native/recomplauncher.json, issue #6): it writes the player's ROM into
    # xmen2.ini's [rom] path (its game_path "rom/path", the path as is) and starts the program bare, so this
    # launcher opens with that ROM. First a new file as it writes one, with the ROM in a folder with spaces and
    # .md (not beside the program, so only [rom] can find it); then the file our launcher has saved since,
    # with that line changed in place, as its INI editor does
    d = out / "recomplauncher"
    picked = d / "My Games" / "X-Men 2 (USA, Europe).md"
    picked.parent.mkdir(parents=True)
    shutil.copy2(rom, picked)
    (d / "xmen2.ini").write_text(f"[rom]\npath={picked}\n")
    r, plan, ini = run_case(exe, d, "press play\n", [])
    same = lambda a, b: a.replace("\\", "/").lower() == str(b).replace("\\", "/").lower()
    ok = plan is not None and same(plan["rom"], picked) and "[launcher]" in ini
    edited = re.sub(r"(?m)^(\s*path\s*=\s*).*$", lambda m: m.group(1) + rom, ini, count=1) if ok else ""
    if ok:
        (d / "xmen2.ini").write_text(edited)
    r2, plan2, ini2 = run_case(exe, d, "press play\n", []) if ok else (r, None, "")
    ok2 = plan2 is not None and same(plan2["rom"], rom)
    check("RecompLauncher's [rom] path", ok and ok2, f"new file: plan rom {plan and plan['rom']}; changed in place: "
          f"{plan2 and plan2['rom']}" + ("" if ok and ok2 else f"\n{r.stderr[-400:]}\n{ini}"))

    # 7b. the Controls page (controls.cpp): a virtual controller's stick press caught for player 1's Z, player 2
    # on W A S D (A S D move from player 1's X Y Z), a second key, player 3 with no controller, a dead zone; all
    # in xmen2.ini, and read back by the next start unchanged
    r, plan, ini = run_case(exe, out / "controls", f"""rom {rom}
vpad
tab controls
wait sleep 0.3
shot {shots / '13_controls.bmp'}
capture 1 z pad
wait sleep 0.2
vpress rightstick 1
wait sleep 0.3
vpress rightstick 0
deadzone 1 50
player 2
keys 2 wasd
bind 2 start key2 key:Left Shift
controller 3 none
wait sleep 0.2
shot {shots / '14_controls_p2.bmp'}
press play
""", [])
    want = ["[player1]", "z = pad:rightstick", "x = pad:leftshoulder", "deadzone = 50", "[player2]",
            "up = key:W, pad:dpup, pad:-lefty", "start = key:Space, key:Left Shift, pad:start", "[player3]",
            "controller = none"]
    missing = [w for w in want if w not in ini]
    r2, plan2, ini2 = run_case(exe, out / "controls", "tab controls\npress play\n", [])
    check("controls", plan is not None and not missing and "[player4]" not in ini and ini2 == ini,
          f"missing {missing}, player 4 written {'[player4]' in ini}, read back unchanged {ini2 == ini}"
          + ("" if not missing else f"\n{r.stderr[-600:]}\n{ini}"))

    # 7c. the controllers' slots and the bindings with SDL's virtual controllers (host_pad_selftest)
    env = {**os.environ, "SDL_VIDEODRIVER": "dummy", "XMEN2_PAD_SELFTEST": "1"}
    r = subprocess.run([str(exe)], capture_output=True, text=True, env=env, timeout=60)
    m = re.search(r"pad selftest: (\d+) of (\d+) steps ok", r.stderr)
    check("pad selftest", r.returncode == 0 and m is not None, m.group(0) if m else r.stderr[-600:])

    # 8-11. join codes (room.cpp): host and joiners on loopback, side by side (XMEN2_ROOM_BIND: no router, no
    # firewall question; RNET_HOST_RELAY_ENDPOINT: the code's "public" address)
    def room_env(port: int, **more: str) -> dict:
        return {"XMEN2_ROOM_BIND": "127.0.0.1", "RNET_HOST_RELAY_ENDPOINT": f"127.0.0.1:{port}", **more}

    def code_game(name: str, players: int, joiners: int, port: int, host_extra: str = "", join_env: dict | None = None,
                  join_wait: str = "seated", shot: bool = False):
        """the host gets a code; `joiners` launchers paste it. -> (host result, [joiner results], code)"""
        hd = out / name / "host"
        code_file = out / name / "code.txt"
        sh = (lambda n: f"shot {shots / n}\n") if shot else (lambda n: "")
        host_script = f"""rom {rom}
tab online
mode host
field players {players}
field port {port}
{sh(f'{name}_1_host_before.bmp')}press host
wait open
code-to {code_file}
{sh(f'{name}_2_host_open.bmp')}{host_extra}"""
        h = start_case(exe, hd, host_script, room_env(port))
        t0 = time.time()
        while not code_file.exists() and h.poll() is None and time.time() - t0 < 30:
            time.sleep(0.05)
        code = code_file.read_text().strip() if code_file.exists() else ""
        js = []
        for k in range(joiners):
            jd = out / name / f"join{k + 1}"
            script = f"""rom {rom}
tab online
mode join
field code {code}
{sh(f'{name}_3_join_pasted.bmp') if k == 0 else ''}press join
wait {join_wait}
{sh(f'{name}_4_join_{join_wait}.bmp') if k == 0 else ''}"""
            js.append(start_case(exe, jd, script, room_env(port, **(join_env or {}))))
            if k + 1 < joiners:
                time.sleep(0.4)   # arrival order = seats
        return finish_case(h, hd), [finish_case(j, out / name / f"join{k + 1}") for k, j in enumerate(js)], code

    # 8. two players: the joiner gets seat 1, the host's game starts by itself once full
    hr, (jr,), code = code_game("code2", 2, 1, 7110, "wait guests1\nshot " + str(shots / "code2_2b_host_full.bmp") + "\n",
                                shot=True)
    hp, jp = hr[1], jr[1]
    ok = hp is not None and hp["netplay"] == ["--netplay", "lan", "--seat", "0", "--seats", "2", "--bind", "127.0.0.1:7110"]
    ok = ok and jp is not None and jp["netplay"][:8] == ["--netplay", "lan", "--seat", "1", "--seats", "2", "--bind", "127.0.0.1:7001"] \
        and jp["netplay"][8] == "--peer" and jp["netplay"][9].endswith(":7110")
    ok = ok and re.fullmatch(r"XM2(-[0-9A-HJKMNP-TV-Z]{1,4})+", code) is not None and "connection = code" in hr[2] \
        and f"code = {code}" in jr[2]
    check("code 2", ok, f"code {code}; host {hp and hp['netplay']}; joiner {jp and jp['netplay']}"
          + ("" if ok else f"\n{hr[0].stderr[-500:]}\n{jr[0].stderr[-500:]}"))

    # 9. four players, three in: the host's START NOW after two joined -> three seats, the seats in arrival order
    hr, jrs, code = code_game("code4", 4, 2, 7120, "wait guests2\npress host\n")
    hp = hr[1]
    jps = [j[1] for j in jrs]
    ok = hp is not None and hp["netplay"] == ["--netplay", "hub", "--seat", "0", "--seats", "3", "--bind", "127.0.0.1:7120"] \
        and hp["options"].get("players") == "3"
    ok = ok and all(p is not None for p in jps) and [p["netplay"][3] for p in jps] == ["1", "2"] \
        and all(p["netplay"][1] == "hub" and p["netplay"][5] == "3" for p in jps)
    check("code 4, start now with 3", ok, f"host {hp and hp['netplay']}; joiners {[p and p['netplay'][:6] for p in jps]}")

    # 10. mistyped codes (a character in the middle; the last one's spare bits; one left off): refused at once
    # with a reason, nothing tried
    flip = lambda c: "A" if c != "A" else "B"  # noqa: E731
    typos = [code[:9] + flip(code[9]) + code[10:], code[:-1] + ("1" if code[-1] == "0" else "0"), code[:-1]]
    bad_ok = []
    for k, bad in enumerate(typos):
        r, plan, ini = run_case(exe, out / f"badcode{k}", f"""rom {rom}
tab online
mode join
field code {bad}
press join
wait error 5
{f"shot {shots / 'code_typo.bmp'}" if k == 0 else ""}
press quit
""", [])
        bad_ok.append(plan is None and "timed out" not in r.stderr)
    check("mistyped code", all(bad_ok), f"{typos} refused: {bad_ok}")

    # 11. another version: the host refuses the joiner, which says so (XMEN2_ROOM_VERSION stands in for it)
    hr, (jr,), code = code_game("codever", 2, 1, 7130, "wait guests1 6\npress quit\n",
                                join_env={"XMEN2_ROOM_VERSION": "xmen2-native-netplay-0"}, join_wait="error")
    ok = jr[1] is None and "timed out" not in jr[0].stderr and hr[1] is None
    check("another version refused", ok, f"joiner plan {jr[1]}, host plan {hr[1]}" + ("" if ok else f"\n{jr[0].stderr[-400:]}"))

    # 12. the hand-off: two launchers settle a room, then their games (played, dummy video) connect and agree
    # on the session ("[netplay] seat I of 2 ... sealed"); then each is stopped by its own PID
    d12 = out / "handoff"
    code_file = d12 / "code.txt"
    h = start_case(exe, d12 / "host", f"rom {rom}\ntab online\nmode host\nfield players 2\nfield port 7150\npress host\n"
                   f"wait open\ncode-to {code_file}\n", room_env(7150), dry=False)
    t0 = time.time()
    while not code_file.exists() and h.poll() is None and time.time() - t0 < 30:
        time.sleep(0.05)
    code = code_file.read_text().strip() if code_file.exists() else ""
    j = start_case(exe, d12 / "join", f"rom {rom}\ntab online\nmode join\nfield code {code}\npress join\n", room_env(7150),
                   dry=False)
    logs = {}
    time.sleep(25)   # the room (a second once full), both games' boot, the session's handshake
    sealed = {}
    for name, p in (("host", h), ("join", j)):
        if p.poll() is None:
            p.kill()   # this PID only
        so, se = p.communicate()
        logs[name] = se
        sealed[name] = re.search(r"^\[netplay\] seat \d of 2 \(lan\), sealed", se, re.M) is not None
    check("code hand-off: the games connect", all(sealed.values()), f"sealed {sealed}"
          + ("" if all(sealed.values()) else f"\nhost: {logs['host'][-400:]}\njoin: {logs['join'][-400:]}"))

    for f in shots.glob("*.bmp"):
        try:
            from PIL import Image
            Image.open(f).save(f.with_suffix(".png"))
            f.unlink()
        except ImportError:
            break
    print(f"{checks[0] - len(fails)}/{checks[0]} pass; screenshots in {shots}")
    return 1 if fails else 0


if __name__ == "__main__":
    raise SystemExit(main())
