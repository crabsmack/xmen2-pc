"""Phase 6 regression test (docs/NOTES.md "Magneto's last phase"): with widescreen_gameplay the Throne
Room's camera stops at CAM_HI ($760 less the margin) and MagnetoBoss_StateWaitCamera waited for x $750,
so Magneto hovered in his box for good. Replays a recorded input (its header gives the session seed and
start scene: AvalonA4's player select, scene 16) headless with RAM dumps every --every syncs, and reads
g_SceneIndex, g_SceneVar3B02 (the camera-zone mask; bit $20 = zone 5) and g_CameraX from them.

Pass: in AvalonA4 (scene 17) zone 5 is held at some dump, i.e. the final fight started. The cases:
  ws1   widescreen_gameplay=1 on this build: must pass
  ws0   widescreen_gameplay=0 on this build: must pass (the cartridge's fight), with --game-rows
  --ref-exe OLD (a build without the fix) adds:
  ref1  widescreen_gameplay=1 on OLD: must FAIL (the test catches the bug)
  ref0  widescreen_gameplay=0 on OLD: its --game-rows must equal ws0's (option off = the cartridge)
The game options are the recording's (its `# options` line), widescreen_gameplay set per case;
--options K=V,... and --players override single ones. At 4:3 (widescreen=0, the regression check)
the margin is 0 and the option changes nothing: every case must pass, and ref1's game rows must equal
ws1's too.
A run that never reaches the boss's wait (MagnetoBoss_StateWaitCamera) says so: the input no longer
gets there, which is no verdict on the fix.

    uv run tools/magneto_check.py OUT [--input game/test_inputs/magneto_final.input] [--ref-exe OLD]
        [--players N] [--options K=V,...] [--every 30]
    uv run tools/magneto_check.py OUT --ref-exe OLD --options widescreen=0   # 4:3
"""
from __future__ import annotations

import argparse
import os
import re
import shutil
import struct
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

OPTIONS = "widescreen=16:9,lives=inf,start_char=wolverine"   # a recording without an `# options` line
TIMEOUT = 1800                      # seconds per run (a 6,389-frame replay takes about a minute)
SCENE_A4 = 17                       # AvalonA4 (Throne_Room); 16 is the player select before it
G_SCENE_INDEX, G_ZONES, G_CAMERA_X = 0xFFCC64, 0xFFC6FE, 0xFFCD72
ZONE5 = 0x20
STATES = {0x0377D6: "WaitCamera", 0x03781C: "FinalSetup", 0x03784E: "FinalFight"}   # MagnetoBoss_State*


def exe() -> Path:
    p = common.BUILD / "Release" / ("XMen2Native.exe" if sys.platform == "win32" else "XMen2Native")
    return p if p.is_file() else common.BUILD / "XMen2Native"


def header(path: Path) -> dict:
    h = {"frames": 0, "session": None, "scene": -1, "options": OPTIONS}
    for line in path.read_text().splitlines():
        if not line.startswith("#"):
            break
        if m := re.match(r"# xmen2 input recording: (\d+) runner frames", line):
            h["frames"] = int(m.group(1))
        elif m := re.match(r"# (session|scene) (-?\d+)", line):
            h[m.group(1)] = int(m.group(2))
        elif m := re.match(r"# options (\S+)", line):
            h["options"] = m.group(1)
    if h["session"] is None or not h["frames"]:
        sys.exit(f"{path}: no '# session' / runner frames header (record with --record-inputs)")
    return h


def parse_options(text: str) -> dict[str, str]:
    return dict(kv.split("=", 1) for kv in text.split(",") if "=" in kv)


def word(ram: bytes, addr: int) -> int:
    return struct.unpack_from(">H", ram, addr & 0xFFFF)[0]


def boss_states(ram: bytes) -> set[str]:
    """the MagnetoBoss state functions found as longs in RAM (its state / resume slots)"""
    found = set()
    for v, name in STATES.items():
        pat, i = struct.pack(">I", v), -1
        while (i := ram.find(pat, i + 1)) >= 0:
            if not i & 1:
                found.add(name)
    return found


def play(runner: Path, d: Path, inp: Path, h: dict, options: str, every: int, rows: bool) -> dict:
    if d.exists():
        shutil.rmtree(d)
    d.mkdir(parents=True)
    local = d / runner.name   # one runner copy per process (CLAUDE.md "Runs by hand")
    shutil.copy2(runner, local)
    env = {**os.environ, "SDL_AUDIODRIVER": "dummy", "XMEN2_DUMP_DIR": str(d),
           "XMEN2_SYNC_DUMP": ",".join(str(s) for s in range(every, h["frames"] + 1, every))}
    cmd = [str(local), str(common.ROM), "--benchmark", str(h["frames"]), "--input-script", str(inp),
           "--session", str(h["session"]), "--options", options,
           *(["--scene", str(h["scene"])] if h["scene"] >= 0 else []),
           *(["--game-rows", str(d / "game.csv")] if rows else [])]
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, env=env, timeout=TIMEOUT)
        rc, err = r.returncode, r.stderr
    except subprocess.TimeoutExpired as e:
        rc, err = f"timeout after {TIMEOUT} s", (e.stderr or b"").decode(errors="replace")
    (d / "stderr.txt").write_text(err)
    local.unlink()
    res = {"rc": rc, "zone5": None, "waited": False, "states": set(), "camx": None, "max_camx": None}
    for f in sorted(d.glob("s*.ram")):
        ram = f.read_bytes()
        if word(ram, G_SCENE_INDEX) != SCENE_A4:
            continue
        sync = int(f.stem[1:])
        cx = struct.unpack_from(">h", ram, G_CAMERA_X & 0xFFFF)[0]
        res["camx"] = cx
        res["max_camx"] = cx if res["max_camx"] is None else max(res["max_camx"], cx)
        st = boss_states(ram)
        res["states"] |= st
        res["waited"] |= "WaitCamera" in st
        if res["zone5"] is None and word(ram, G_ZONES) & ZONE5:
            res["zone5"] = sync
    for f in [*d.glob("s*.ram"), *d.glob("s*.vram")]:
        f.unlink()
    return res


def rows_equal(a: Path, b: Path) -> str:
    ra, rb = a.read_text().splitlines(), b.read_text().splitlines()
    k = next((i for i, (x, y) in enumerate(zip(ra, rb)) if x != y), None if len(ra) == len(rb) else min(len(ra), len(rb)))
    return "equal" if k is None else f"differ at row {k} of {len(ra)}"


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("--input", default=str(common.GAME / "test_inputs" / "magneto_final.input"))
    ap.add_argument("--ref-exe", help="a build without the fix: ref1 must fail, ref0's game rows must equal ws0's")
    ap.add_argument("--players", type=int, help="players=N (a four-pad input for 3-4)")
    ap.add_argument("--options", default="", help="K=V,... over the recording's options (widescreen_gameplay: per case)")
    ap.add_argument("--every", type=int, default=30, help="syncs between RAM dumps")
    a = ap.parse_args()
    common.ensure_rom()
    inp = Path(a.input).resolve()
    h = header(inp)
    out = Path(a.out).resolve()
    o = parse_options(h["options"]) | parse_options(a.options) | ({"players": str(a.players)} if a.players else {})
    o.pop("widescreen_gameplay", None)
    opts = ",".join(f"{k}={v}" for k, v in o.items())
    cases = {"ws1": (exe(), 1), "ws0": (exe(), 0)}   # name: runner, widescreen_gameplay
    if a.ref_exe:
        cases |= {"ref1": (Path(a.ref_exe).resolve(), 1), "ref0": (Path(a.ref_exe).resolve(), 0)}
    four3 = o.get("widescreen", "auto") != "16:9"   # no margin (headless auto is off): the option changes nothing
    expect = {"ws1": True, "ws0": True, "ref1": four3, "ref0": True}   # zone 5 reached

    def one(name: str):
        runner, ws = cases[name]
        return name, play(runner, out / name, inp, h, f"widescreen_gameplay={ws},{opts}", a.every, ws == 0 or four3)

    print(f"{inp.name}: {h['frames']} frames, scene {h['scene']}, session {h['session']}, options {opts}")
    bad = 0
    with ThreadPoolExecutor(len(cases)) as ex:
        results = dict(ex.map(one, cases))
    for name, res in results.items():
        if res["rc"] != 0:
            bad += 1
            why = res["rc"] if isinstance(res["rc"], str) else f"exit {res['rc']}"
            print(f"{name}: FAILED runner {why} ({out / name / 'stderr.txt'})")
            continue
        if not res["waited"] and res["zone5"] is None:
            bad += 1
            print(f"{name}: NO VERDICT: the input never reached MagnetoBoss_StateWaitCamera")
            continue
        passed = res["zone5"] is not None
        ok = passed == expect[name]
        bad += not ok
        cx = lambda v: "-" if v is None else f"${v & 0xFFFF:X}"
        print(f"{name}: {'pass' if passed else 'FAIL'} ({'as expected' if ok else 'NOT AS EXPECTED'}): "
              + (f"zone 5 at sync {res['zone5']}" if passed else "zone 5 never held")
              + f", g_CameraX at the last AvalonA4 dump {cx(res['camx'])} (max {cx(res['max_camx'])}), boss states seen "
              + (", ".join(sorted(res["states"])) or "none"))
    for ws in (0, 1) if four3 else (0,):
        if f"ref{ws}" in results and all(results[k]["rc"] == 0 for k in (f"ws{ws}", f"ref{ws}")):
            eq = rows_equal(out / f"ref{ws}" / "game.csv", out / f"ws{ws}" / "game.csv")
            bad += eq != "equal"
            print(f"game rows, widescreen_gameplay={ws}, this build vs --ref-exe: {eq}")
    return 1 if bad else 0


if __name__ == "__main__":
    raise SystemExit(main())
