"""Quick saves and rewind (platform/native/rewind.cpp; docs/NOTES.md "Quick saves and rewind"), headless.

    uv run tools/rewindcheck.py [--only tas,two,mp] [--out DIR] [--json]

Each configuration is played once plain (the reference) and then with keys pressed by XMEN2_HOTKEYS
(STEP:ACTION at the STEP-th sync point of the run): a quick save and a load later on, a rewind held
for some frames, slots switched and an empty slot loaded, a rewind past the ring's oldest frame
(XMEN2_REWIND_SECONDS=2). A run passes when every sync row, canonical digest row and frame's picture
hash (--hash-frames 1) it writes, the ones written again after a load or a rewind step included, is
the reference's for that sync point or frame; each frame is written as many times as the keys make
the game play it (so the load or rewind did happen, by exactly as many frames); the final counters
of the game's sound and players agree; and rewind.mismatch is 0 (each replayed frame's digest is the
one the ring kept). Then the negative control: the same keys with one savestate section left out
of every load (XMEN2_REWIND_SKIP), which must be caught. Also XMEN2_REWIND_SELFTEST (the ring's
encoding) and XMEN2_PAD_SELFTEST (the Rewind binding among the controls).

Configurations: tas (the TAS by runner frame in the session machine, as a played run, to 6,000),
two (scenario-two-player, the frame-tick V-ints of two players stepping), mp (scene 1 with four
players: players 3-4's sections). Exits 1 on any failure or a section not caught."""
import argparse
import json
import os
import re
import subprocess
import sys
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402
import harness  # noqa: E402
import xmen2check  # noqa: E402

SECTIONS = ["fiber", "arena", "players34", "machine", "platform", "sound", "snd2", "snd_music", "snd_parts", "p34video"]


def configs(refs: Path, out: Path) -> dict:
    """name: (script, extra args, frames, [(case, hotkeys, env, expected plays: frame -> times)])"""
    out.mkdir(parents=True, exist_ok=True)

    def by_frame(src: Path, dst: Path) -> Path:
        dst.write_text("".join(l for l in src.read_text().splitlines(True) if not l.strip().upper().startswith("EXIT")),
                       newline="\n")
        return dst

    tas = by_frame(refs / "tas.input", out / "tas.input")
    two = by_frame(refs / "scenario-two-player.input", out / "two.input")
    mp = common.GAME / "mp_inputs" / "scene1.input"

    def cases(a: int, b: int, r_at: int, r: int, e_at: int) -> list:
        """save at frame a, load at b; rewind r frames at r_at; slots and an empty load; a rewind of 400 at e_at
        with a 2-second ring (it stops at the ring's oldest frame, 119 back)"""
        plays_load = {f: 2 for f in range(a + 1, b + 1)}
        # rewind of r at frame B: frames B-1..B-r shown again (each replayed once), then B-r+1..B played again
        plays_rw = {f: 1 for f in range(r_at - r, r_at + 1)}
        for f in range(r_at - r, r_at):
            plays_rw[f] += 1
        for f in range(r_at - r + 1, r_at + 1):
            plays_rw[f] += 1
        ring = 2 * 60 - 1
        plays_end = {f: 1 for f in range(e_at - ring, e_at + 1)}
        for f in range(e_at - ring, e_at):
            plays_end[f] += 1
        for f in range(e_at - ring + 1, e_at + 1):
            plays_end[f] += 1
        # slot 2 saved at a, slot 1 at b, slot 3 (empty) tried, slot 2 loaded at b+300: back to a
        plays_slots = {f: 2 for f in range(a + 1, b + 301)}
        return [
            ("save-load", f"{a}:save,{b}:load", {}, plays_load),
            ("rewind", f"{r_at}:rewind{r}", {}, plays_rw),
            ("slots", f"{a}:slot2,{a}:save,{b}:slot1,{b}:save,{b + 100}:slot3,{b + 100}:load,{b + 300}:slot2,{b + 300}:load",
             {}, plays_slots),
            ("ring-end", f"{e_at}:rewind400", {"XMEN2_REWIND_SECONDS": "2"}, plays_end),
        ]

    return {
        "tas": (tas, ["--session", "1"], 6000, cases(2500, 3400, 4000, 300, 5000)),
        "two": (two, ["--session", "1"], 4900, cases(1500, 2100, 3000, 240, 4000)),
        "mp": (mp, ["--session", "1", "--scene", "1", "--options", "players=4"], 4433, cases(1200, 1900, 2800, 200, 3800)),
    }


def rows(text: str, key: int) -> dict:
    """the lines of a csv or the [FBHASH] log by sync point / frame: {key: [lines]}"""
    out = defaultdict(list)
    for l in text.splitlines():
        if l.startswith("[FBHASH]"):
            m = re.search(r"frame=(\d+)", l)
            if m:
                out[int(m.group(1))].append(l)
        elif l and l[0].isdigit():
            out[int(l.split(",")[key])].append(l)
    return out


def run(src: Path, d: Path, script: Path, extra: list[str], frames: int, env_more: dict) -> dict:
    d.mkdir(parents=True, exist_ok=True)
    rd = d / "runner"
    rd.mkdir(exist_ok=True)
    xmen2check.copy_runner(src, rd)
    env = {**os.environ, "SDL_AUDIODRIVER": "dummy", "GENESIS_NO_LAUNCHER": "1", **xmen2check.crash_env(src),
           "XMEN2_SYNC_CSV": str(d / "sync.csv"), **env_more}
    r = subprocess.run([str(rd / src.name), str(common.ROM), "--benchmark", str(frames), "--input-script", str(script),
                        *extra, "--digest", str(d / "digest.csv"), "--hash-frames", "1", "--hash-out", str(d / "fbhash.log")],
                       cwd=rd, env=env, capture_output=True, text=True, errors="replace", timeout=3600)
    (d / "runner.log").write_text(r.stderr, encoding="utf-8")
    c = re.search(r"\[xmen2\] counters (\{.*\})", r.stderr)
    counters = json.loads(c.group(1)) if c else {}
    read = lambda f: (d / f).read_text() if (d / f).exists() else ""
    return {"code": r.returncode, "sync": rows(read("sync.csv"), 0), "digest": rows(read("digest.csv"), 0),
            "fb": rows(read("fbhash.log"), 0), "counters": counters,
            "said": [l for l in r.stderr.splitlines() if l.startswith(("[rewind]", "[crash]"))][-3:]}


def compare(ref: dict, got: dict, plays: dict | None) -> list[str]:
    """what differs from the reference ([] = none); plays: frame -> times each frame's picture is written"""
    bad = [f"exit {got['code']}"] if got["code"] != ref["code"] else []
    for kind in ("sync", "digest", "fb"):
        a, b = ref[kind], got[kind]
        if set(a) != set(b):
            bad.append(f"{kind}: {len(set(a) ^ set(b))} keys only on one side")
        wrong = [k for k in b if k in a and any(l != a[k][0] for l in b[k])]
        if wrong:
            bad.append(f"{kind}: {len(wrong)} differ, first at {min(wrong)}")
    if plays is not None:
        times = Counter({k: len(v) for k, v in got["fb"].items()})
        off = [f for f in set(times) | set(plays) if times.get(f, 0) != plays.get(f, 1) and f in ref["fb"]]
        if off:
            f = min(off)
            bad.append(f"frames played {len(off)} times off (first: frame {f} {times.get(f, 0)}x, meant {plays.get(f, 1)}x)")
    for k in ("sound", "mp"):
        if ref["counters"].get(k) != got["counters"].get(k):
            bad.append(f"counters.{k}")
    rw = got["counters"].get("rewind", {})
    if rw.get("mismatch") or rw.get("bad"):
        bad.append(f"rewind.mismatch {rw.get('mismatch')}, bad {rw.get('bad')}")
    return bad


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--only", default="tas,two,mp")
    ap.add_argument("--refs", default="", help="phase.py's build/phase_traces (default: $XMEN2_REFS, harness.py's)")
    ap.add_argument("--out", default="")
    ap.add_argument("--no-negative", action="store_true", help="skip the sections left out")
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args()
    common.ensure_rom()
    out = Path(a.out).resolve() if a.out else common.BUILD / "rewindcheck"
    src = xmen2check.exe_path(xmen2check.NATIVE_EXE)
    if not src.is_file():
        sys.exit(f"no runner at {src}: run `xmen2check.py build`")
    res = {"selftest": {}, "configs": {}, "caught": {}, "not_caught": []}
    for name, var in (("encoding", "XMEN2_REWIND_SELFTEST"), ("pads", "XMEN2_PAD_SELFTEST")):
        r = subprocess.run([str(src)], env={**os.environ, var: "1"}, capture_output=True, text=True, errors="replace")
        res["selftest"][name] = r.returncode == 0
        print(f"selftest {name}: {'ok' if r.returncode == 0 else 'FAILED: ' + r.stderr.strip()[-300:]}", flush=True)
    cfgs = configs(harness.refs_dir(a.refs), out)
    caught = defaultdict(list)
    jobs = {}   # (config, case or "ref", section or "") -> future
    with ThreadPoolExecutor(max(1, (os.cpu_count() or 4) - 2)) as pool:
        for name in a.only.split(","):
            script, extra, frames, cases = cfgs[name]
            jobs[(name, "ref", "")] = pool.submit(run, src, out / name / "ref", script, extra, frames, {})
            for case, keys, env, plays in cases:
                jobs[(name, case, "")] = pool.submit(run, src, out / name / case, script, extra, frames,
                                                     {"XMEN2_HOTKEYS": keys, **env})
                if a.no_negative or case not in ("save-load", "rewind"):
                    continue
                for sec in SECTIONS:
                    jobs[(name, case, sec)] = pool.submit(run, src, out / name / f"{case}-skip-{sec}", script, extra, frames,
                                                          {"XMEN2_HOTKEYS": keys, "XMEN2_REWIND_SKIP": sec, **env})
        for name in a.only.split(","):
            script, extra, frames, cases = cfgs[name]
            ref = jobs[(name, "ref", "")].result()
            print(f"{name}: {frames} frames, exit {ref['code']}", flush=True)
            cr = res["configs"][name] = {}
            for case, keys, env, plays in cases:
                got = jobs[(name, case, "")].result()
                d = compare(ref, got, plays)
                rw = got["counters"].get("rewind", {})
                cr[case] = {"differs": d, "rewind": rw}
                print(f"  {case}: {'as the reference' if not d else 'DIFFERS: ' + '; '.join(d)}  (steps {rw.get('steps')}, "
                      f"loads {rw.get('loads')}, empty loads {rw.get('empty_loads')}, {rw.get('bytes_per_frame')} B/frame)",
                      flush=True)
                for sec in SECTIONS:
                    if (name, case, sec) not in jobs:
                        continue
                    d = compare(ref, jobs[(name, case, sec)].result(), plays)
                    cr[f"{case}-skip-{sec}"] = d
                    if d:
                        caught[sec].append(f"{name}/{case}")
                    print(f"    {sec} left out: {'caught (' + '; '.join(d) + ')' if d else 'not caught'}", flush=True)
    res["caught"] = dict(caught)
    res["not_caught"] = [] if a.no_negative else [s for s in SECTIONS if s not in caught]
    ok = all(res["selftest"].values()) and all(not v["differs"] for c in res["configs"].values()
                                                for k, v in c.items() if isinstance(v, dict)) and not res["not_caught"]
    res["ok"] = ok
    out.mkdir(parents=True, exist_ok=True)
    (out / "rewindcheck.json").write_text(json.dumps(res, indent=1))
    print(f"rewindcheck: {'pass' if ok else 'FAIL'}; sections not caught: {', '.join(res['not_caught']) or 'none'}")
    if a.json:
        print(json.dumps({k: v for k, v in res.items() if k != "configs"}))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
