"""Render a recorded take of the native port: the picture, lossless, and the sound as two stems, the
music and the sound effects (platform/native/capture.cpp; docs/NOTES.md "Capture").

    # record while playing (keyboard/controller, or online: every seat can record the shared game)
    build/Release/XMen2Native.exe xmen2.bin --record-inputs take.input [--scene N] [--options ...]

    uv run tools/capture.py render take.input OUT [--options widescreen=16:9] [--frames N]

OUT gets video.mkv (FFV1, every runner frame at 53693175/896040 fps, on a fixed canvas), music.wav and
sfx.wav (223,721 Hz stereo from runner frame 1, so frame F starts at (F-1)/59.9227 s; each with the
chips' DC bias), and take.json. The two stems are rendered in two runs of the same input, which must
end with the same game: their digests are compared, and a mismatch is an error. --options adds
presentation options (widescreen, high_fps is not captured: the capture is the game's frames); the
take's game options, session seed and start scene come from its header.
"""
from __future__ import annotations

import argparse
import json
import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

FPS = 53693175 / 896040


def read_header(path: Path) -> dict:
    h = {"session": None, "scene": -1, "options": "", "frames": 0}
    for line in path.read_text().splitlines():
        if not line.startswith("#"):
            continue
        k, _, v = line[1:].strip().partition(" ")
        if k == "session":
            h["session"] = int(v)
        elif k == "scene":
            h["scene"] = int(v)
        elif k == "options":
            h["options"] = v.strip()
        elif k == "xmen2" and "runner frames" in v:
            h["frames"] = int(v.split(":")[1].split()[0])
    if h["session"] is None:
        sys.exit(f"{path}: no '# session' line (record with --record-inputs)")
    return h


def native_exe() -> Path:
    exe = common.BUILD / "Release" / ("XMen2Native.exe" if sys.platform == "win32" else "XMen2Native")
    return exe if exe.is_file() else common.BUILD / "XMen2Native"


def render(a) -> int:
    take = Path(a.take).resolve()
    out = Path(a.out).resolve()
    out.mkdir(parents=True, exist_ok=True)
    h = read_header(take)
    frames = a.frames or h["frames"]
    opts = ",".join(o for o in (h["options"], a.options) if o)
    exe = Path(a.exe) if a.exe else native_exe()
    rom = common.ensure_rom()

    def run(stem: str):
        env = {**os.environ, "SDL_AUDIODRIVER": "dummy", "XMEN2_STEM": stem, "XMEN2_WAV": str(out / f"{stem}.wav")}
        if stem == "music":
            env["XMEN2_CAPTURE"] = str(out / "video.mkv")
        cmd = [str(exe), str(rom), "--benchmark", str(frames), "--input-script", str(take),
               "--session", str(h["session"]), "--digest", str(out / f"digest-{stem}.csv")]
        if h["scene"] >= 0:
            cmd += ["--scene", str(h["scene"])]
        if opts:
            cmd += ["--options", opts]
        r = subprocess.run(cmd, env=env, capture_output=True, text=True)
        (out / f"{stem}.log").write_text(r.stdout + r.stderr)
        return stem, r.returncode

    with ThreadPoolExecutor(2) as ex:
        results = dict(ex.map(run, ("music", "sfx")))
    if any(results.values()):
        print(f"render failed: {results} (logs in {out})")
        return 1
    same = (out / "digest-music.csv").read_bytes() == (out / "digest-sfx.csv").read_bytes()
    info = {"take": str(take), "frames": frames, "fps": FPS, "wav_rate": 223721, "options": opts,
            "session": h["session"], "scene": h["scene"], "digests_equal": same}
    (out / "take.json").write_text(json.dumps(info, indent=2))
    print(json.dumps(info))
    return 0 if same else 1


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("render", help="a take's picture and its music/SFX stems")
    r.add_argument("take")
    r.add_argument("out")
    r.add_argument("--options", default="", help="presentation options to add (e.g. widescreen=16:9)")
    r.add_argument("--frames", type=int, default=0, help="render only this many runner frames")
    r.add_argument("--exe", default="", help="the native runner (default: build/Release/XMen2Native.exe)")
    a = ap.parse_args()
    return render(a)


if __name__ == "__main__":
    sys.exit(main())
