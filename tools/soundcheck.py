"""music_voices' check (docs/NOTES.md "music_voices"): every song the music machines play gets the notes
the first machine gives it when it has the chips, and the track slots, to itself.

    uv run tools/soundcheck.py [--only boot,18] [--players 4,3] [--seeds 1,40] [--options ";full_rate_2p=1"] [--jobs N]

For each stage input (game/mp_inputs: boot.input and scene<N>.input), number of players, session seed
and option set, four runs of XMen2Native (headless, the session machine, where the sound chips run),
each logging every note it counts (XMEN2_KEYON_LOG: machine, frame, FM key-on or PSG note, song):

- the take: music_voices=1 (the game's arrangement of the parts). Its counters line's "sound" names
  the songs the game started as effects.
- the full take: music_voices=full (the default: each part in full, as one player hears it).
- the parts' reference: music_voices=0, XMEN2_BED=1 (effects that only play notes never reach the
  driver; those that start or stop songs, set the tempo, a variable or the master volume do, muted),
  XMEN2_SOLO = the part songs: the first machine plays players 1 and 2's parts with every voice and
  track slot free for them.
- the music's reference: the same with XMEN2_SOLO = every song that is neither a part nor an effect.

Each part song's notes on the parts machine and the second (players 3-4's, which plays them in both
runs) must be its notes on the first machine and the second in the parts' reference: note for note,
each within SLACK frames of its reference, and only notes in the last SLACK frames may be missing on
either side. The references count only if the game is the take's: the three runs' game rows
(--game-rows: the game's state without the sound machine's, whose RAM XMEN2_BED and XMEN2_SOLO
change) must be equal (the game reads the driver's variables), the full take's too. The full take's
part songs, on the parts machine and the full-parts machine (players 3-4's; the second machine is
silent there), must have every note of the parts' reference within SLACK frames (a muted track only
skips notes), with no voice stolen on either machine, and (with parts played) notes more: the
tracks the thinned parts mute. Exit 1 on any difference.

Every other music song's notes on the music machine against the music's reference are compared the
same way and printed, but not counted ("not counted: ..."): a driver polls its command ring between
tempo ticks, and how long a tick takes depends on the tracks it runs, so each machine takes a command
up to a few frames before or after the first machine (measured: a stop-all three frames late, five
more notes of $08), and a note at a stop or start comes or goes; a PSG note is counted when a silent
channel gets a volume, so the timing also decides whether two notes count as one. The parts have shown
none of this (every stage input, players 4 and 3, seeds 1, 40, 78, both rates: 2026-10-25).
"""
from __future__ import annotations

import argparse
import concurrent.futures
import json
import os
import re
import subprocess
import sys
import tempfile
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

INPUTS = common.GAME / "mp_inputs"
SLACK = 8   # frames


def part_song(s: int) -> bool:
    """sound2.cpp's part_song: $27/$25's parts, $48's, $64's"""
    return (0x28 <= s <= 0x2F and s != 0x2E) or (0x49 <= s <= 0x50 and s != 0x4F) or 0x65 <= s <= 0x6B


PARTS = [s for s in range(256) if part_song(s)]


def exe() -> Path:
    p = common.BUILD / "Release" / ("XMen2Native.exe" if sys.platform == "win32" else "XMen2Native")
    return p if p.is_file() else common.BUILD / "XMen2Native"


def frames(inp: Path) -> int:
    return sum(int(m.group(1)) for m in re.finditer(r"^WAIT (\d+)", inp.read_text(), re.M))


def run(inp: Path, scene: int | None, seed: int, options: str, env_extra: dict, td: str, tag: str):
    """one run: its counters' "sound", its game rows, its notes {(machine, song, kind): [frames]}"""
    rows, log = Path(td) / f"{tag}.csv", Path(td) / f"{tag}.notes"
    cmd = [str(exe()), str(common.ROM), "--benchmark", str(frames(inp)), "--input-script", str(inp),
           "--session", str(seed), "--options", options, "--game-rows", str(rows)]
    if scene is not None:
        cmd += ["--scene", str(scene)]
    env = {**os.environ, "SDL_AUDIODRIVER": "dummy", "XMEN2_KEYON_LOG": str(log), **env_extra}
    for k in ("XMEN2_BED", "XMEN2_SOLO", "XMEN2_STEM"):
        if k not in env_extra:
            env.pop(k, None)
    r = subprocess.run(cmd, capture_output=True, text=True, env=env, timeout=3600)
    m = re.search(r"\[xmen2\] counters (\{.*\})", r.stdout + r.stderr)
    if r.returncode or not m:
        raise RuntimeError(f"{tag}: exit {r.returncode}: {(r.stdout + r.stderr).strip()[-300:]}")
    notes: dict[tuple[str, int, str], list[int]] = defaultdict(list)
    for line in log.read_text().splitlines() if log.is_file() else ():   # opened at the first note
        who, f, kind, song = line.split(",")
        notes[(who, int(song), kind)].append(int(f))
    return json.loads(m.group(1))["sound"], rows.read_text(), notes


def gather(notes: dict, machines: tuple[str, ...], keep) -> dict[tuple[int, str], list[int]]:
    out: dict[tuple[int, str], list[int]] = defaultdict(list)
    for (who, song, kind), fs in notes.items():
        if who in machines and keep(song):
            out[(song, kind)] += fs
    return {k: sorted(v) for k, v in out.items()}


def unmatched(got: list[int], ref: list[int], end: int) -> tuple[list[int], list[int]]:
    """the notes of each side with none on the other within SLACK frames, but those in the last SLACK
    frames (greedy, in order: both lists sorted)"""
    used = [False] * len(ref)
    extra, j = [], 0   # ref[j:] are not used yet
    for f in got:
        while j < len(ref) and ref[j] < f - SLACK:
            j += 1   # too early for this note and every later one: missing
        if j < len(ref) and ref[j] <= f + SLACK:
            used[j] = True
            j += 1
        elif f <= end - SLACK:
            extra.append(f)
    missing = [f for f, u in zip(ref, used) if not u and f <= end - SLACK]
    return extra, missing


def compare(what: str, got: dict, ref: dict, end: int) -> list[str]:
    out = []
    for key in sorted({*got, *ref}):
        extra, missing = unmatched(got.get(key, []), ref.get(key, []), end)
        if extra or missing:
            song, kind = key
            out.append(f"{what} ${song & 0xFF:02X} {kind} {len(got.get(key, []))} vs {len(ref.get(key, []))}"
                       f" (missing at {missing[:4]}, extra at {extra[:4]})")
    return out


def counts(d: dict) -> str:
    by: dict[int, list[int]] = defaultdict(lambda: [0, 0])
    for (song, kind), fs in d.items():
        by[song][kind == "psg"] += len(fs)
    return " ".join(f"${s & 0xFF:02X} {v[0]}/{v[1]}" for s, v in sorted(by.items()))


def superset(what: str, got: dict, ref: dict, end: int) -> tuple[list[str], int]:
    """music_voices=full's parts against the thinned reference: every reference note is there (a muted
    track only skips notes), the unmuted tracks' notes are extra; and the extra notes' count"""
    out, n = [], 0
    for key in sorted({*got, *ref}):
        extra, missing = unmatched(got.get(key, []), ref.get(key, []), end)
        n += len(extra)
        if missing:
            out.append(f"{what} ${key[0] & 0xFF:02X} {key[1]} {len(got.get(key, []))} vs {len(ref.get(key, []))}"
                       f" (missing at {missing[:4]})")
    return out, n


def case(name: str, scene: int | None, players: int, seed: int, extra: str) -> tuple[bool, str]:
    inp = INPUTS / ("boot.input" if scene is None else f"scene{scene}.input")
    options = f"players={players}" + (f",{extra}" if extra else "")
    label = f"{name} players {players} seed {seed}" + (f" {extra}" if extra else "")
    with tempfile.TemporaryDirectory(prefix="soundcheck_") as td:
        try:
            take, g0, n0 = run(inp, scene, seed, options + ",music_voices=1", {}, td, "take")
            full, g3, n3 = run(inp, scene, seed, options + ",music_voices=full", {}, td, "full")
            fx = {int(s, 16) for s in take["effects"]}
            music = [s for s in range(256) if not part_song(s) and s not in fx]
            off = options + ",music_voices=0"
            _, g1, n1 = run(inp, scene, seed, off, {"XMEN2_BED": "1", "XMEN2_SOLO": ",".join(f"{s:02X}" for s in PARTS)},
                            td, "parts")
            _, g2, n2 = run(inp, scene, seed, off, {"XMEN2_BED": "1", "XMEN2_SOLO": ",".join(f"{s:02X}" for s in music)},
                            td, "music")
        except RuntimeError as e:
            return False, f"{label}: a run failed: {e}"
    if take["music_voices"] != 1 or take["full_parts"] != 0 or full["full_parts"] != 1:
        return False, f"{label}: music_voices is not 1 in the take or full in the full take"
    if g1 != g0 or g2 != g0 or g3 != g0:
        return False, f"{label}: a reference's game differs from the take's (game rows), no reference"
    is_music = lambda s: s >= 0 and not part_song(s) and s not in fx   # noqa: E731
    is_part = lambda s: s >= 0 and part_song(s)   # noqa: E731
    got_p = gather(n0, ("snd_parts", "snd2"), is_part)
    ref_p = gather(n1, ("m1", "snd2"), is_part)
    got_m = gather(n0, ("snd_music",), is_music)
    ref_m = gather(n2, ("m1",), is_music)
    end = frames(inp)
    bad, jitter = compare("parts", got_p, ref_p, end), compare("music", got_m, ref_m, end)
    # full: players 1-2's parts on the parts machine, 3-4's on the full-parts machine (the second silent)
    got_f = gather(n3, ("snd_parts", "snd_parts34"), is_part)
    bad_f, more = superset("full parts", got_f, ref_p, end)
    stolen = {f"{m} ${k}": v["stolen"] for m in ("parts", "parts34") for k, v in full[m]["songs"].items() if v["stolen"]}
    if stolen:
        bad_f.append(f"full parts: voices stolen {stolen}")
    if ref_p and players >= 2 and not more:
        bad_f.append("full parts: no note more than the thinned parts")
    bad += bad_f
    summary = f"parts {counts(got_p)}; full parts {counts(got_f)}; music {counts(got_m)}"
    return not bad, (f"{label}: {'ok' if not bad else 'DIFFERS: ' + '; '.join(bad)}"
                     + (f" (not counted: {'; '.join(jitter)})" if jitter else "") + f" ({summary})")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--only", default="", help="boot and/or scene numbers, comma-separated (default: every input)")
    ap.add_argument("--players", default="4", help="comma-separated")
    ap.add_argument("--seeds", default="1,40", help="session seeds (the characters players 2-4 draw), comma-separated")
    ap.add_argument("--options", default=";full_rate_2p=1",
                    help="option sets added to players=N, ';'-separated (default: none, and full_rate_2p=1)")
    ap.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 4) - 2))
    a = ap.parse_args()
    common.ensure_rom()
    items: list[tuple[str, int | None]] = []
    for p in sorted(INPUTS.glob("*.input"), key=lambda p: -1 if p.stem == "boot" else int(p.stem[5:])):
        items.append(("boot", None) if p.stem == "boot" else (p.stem, int(p.stem[5:])))
    if a.only:
        want = set(a.only.split(","))
        items = [(n, s) for n, s in items if (n if s is None else str(s)) in want or n in want]
    cases = [(n, s, int(p), int(seed), o) for n, s in items for p in a.players.split(",") for seed in a.seeds.split(",")
             for o in a.options.split(";")]
    ok = 0
    with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:   # a case's three runs one after another
        for good, line in ex.map(lambda c: case(*c), cases):
            ok += good
            print(line, flush=True)
    print(f"soundcheck: {ok} of {len(cases)} cases ok")
    return 0 if ok == len(cases) else 1


if __name__ == "__main__":
    raise SystemExit(main())
