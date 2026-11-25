"""[rewind] sound = reversed (platform/native/host_sdl.cpp host_audio; docs/NOTES.md "Quick saves and rewind"), headless.

    uv run tools/rewindsound.py [--out DIR]

The TAS is played to frame 4,300 three times with the generated sound taped (XMEN2_WAV, the mixed stream before
the device, bit-exact): plain; with a rewind of 120 frames held at frame 4,000 and [rewind] sound = reversed; the
same with sound = silent (the control). A rewind of R at frame C replays frames C-1 down to C-R, then plays on from
C-R+1. Reversed passes when the sound after frame C's is the plain run's frames C-R..C-1 backwards, sample for
sample but for the fade at each end (an eighth of a frame), and the run ends on the plain run's sound from frame
C-R+1's on (its first eighth faded in); silent passes when the same stretch is silence. Both: rewind.steps R,
rewind.mismatch 0."""
import argparse
import json
import sys
import wave
from array import array
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402
import harness  # noqa: E402
import rewindcheck  # noqa: E402
import xmen2check  # noqa: E402

FRAMES, AT, R = 4300, 4000, 120
FRAME = 3734            # PSG-rate samples in a frame (3,733 or 3,734)
FADE = FRAME // 8 + 2   # past a fade, with a sample to spare


def tape(p: Path) -> array:
    """the WAV's stereo samples, a 32-bit word each (so a reversal keeps left and right together)"""
    with wave.open(str(p)) as w:
        return array("i", w.readframes(w.getnframes()))


def first_diff(a: array, b: array) -> int:
    lo, hi = 0, min(len(a), len(b))   # a[:lo] == b[:lo]; the first difference is below hi
    if a[:hi] == b[:hi]:
        return hi
    while hi - lo > 1:
        mid = (lo + hi) // 2
        if a[:mid] == b[:mid]:
            lo = mid
        else:
            hi = mid
    return lo


def common_tail(a: array, b: array) -> int:
    ra, rb = array("i", reversed(a)), array("i", reversed(b))
    return first_diff(ra, rb)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default="")
    a = ap.parse_args()
    common.ensure_rom()
    out = Path(a.out).resolve() if a.out else common.BUILD / "rewindsound"
    src = xmen2check.exe_path(xmen2check.NATIVE_EXE)
    script = rewindcheck.configs(harness.refs_dir(""), out)["tas"][0]
    runs = {}
    for name, env in (("plain", {}),
                      ("reversed", {"XMEN2_HOTKEYS": f"{AT}:rewind{R}", "XMEN2_REWIND_SOUND": "reversed"}),
                      ("silent", {"XMEN2_HOTKEYS": f"{AT}:rewind{R}", "XMEN2_REWIND_SOUND": "silent"})):
        d = out / name
        r = rewindcheck.run(src, d, script, ["--session", "1"], FRAMES, {**env, "XMEN2_WAV": str(d / "sound.wav")})
        runs[name] = (r, tape(d / "sound.wav"))
        print(f"{name}: exit {r['code']}, {len(runs[name][1])} samples", flush=True)
    A = runs["plain"][1]
    res = {}
    for name in ("reversed", "silent"):
        r, B = runs[name]
        d = first_diff(A, B)            # where frame C-1's replay starts (L(C))
        tail = common_tail(A, B)        # frame C-R+1 on, as the plain run's (but its faded-in start)
        stretch = len(B) - tail - d     # the R replayed frames, and the faded part of C-R+1's
        info = {"first_diff": d, "stretch": stretch, "stretch_frames": round(stretch / (FRAME - 0.5), 2), "tail": tail}
        if name == "reversed":
            # frame C's length nf: then L(C-1) = d - nf, and B[d + j] = A[L(C-1) - 1 - j] across the stretch
            inner = B[d + FADE: d + R * FRAME - 2 * FRAME]
            nf = next((n for n in range(3700, 3770)
                       if array("i", reversed(A[d - n - FADE - len(inner): d - n - FADE])) == inner), None)
            info["frame_c_len"] = nf
            ok = nf is not None
        else:
            ok = all(v == 0 for v in B[d: d + stretch - FRAME])
            info["silence"] = ok
        rw = r["counters"].get("rewind", {})
        info["rewind"] = {k: rw.get(k) for k in ("steps", "mismatch", "sound")}
        ok = ok and r["code"] == 0 and rw.get("steps") == R and not rw.get("mismatch") and rw.get("sound") == name \
            and R - 1 <= info["stretch_frames"] <= R + 1
        info["ok"] = ok
        res[name] = info
        print(f"{name}: {'ok' if ok else 'FAIL'} {json.dumps(info)}", flush=True)
    res["ok"] = all(v["ok"] for v in res.values())
    (out / "rewindsound.json").write_text(json.dumps(res, indent=1))
    print(f"rewindsound: {'pass' if res['ok'] else 'FAIL'}")
    return 0 if res["ok"] else 1


if __name__ == "__main__":
    sys.exit(main())
