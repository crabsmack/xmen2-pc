"""State files across processes, with a negative control (docs/NOTES.md "TAS state files").

    uv run tools/statecheck.py [--only verify,tas,session,mp] [--out DIR] [--json]

A run saves state files at a few frames (the runners' --state-at); each is loaded by a process of
its own runner copy (--state-load) and must play on exactly as the run that saved it: the same
files (sync rows, the canonical digest; verify's JSON), counters and final state hash. Then once per
section left out of the load (XMen2Native --state-skip, the engine's GENESIS_RB_NEGCTL_SKIP,
XMEN2_STATE_SKIP_TESTING for verify's testing state), which must change something; native runs also
save at a later frame, where each section's hash (XMEN2_STATE_SECTIONS; not the fiber's raw stack)
is compared too. Last, another process layout must be refused: another /GS cookie, the build's own
runner (ASLR); and loaded anyway (XMEN2_STATE_PLACE_UNCHECKED), what that does.

Configurations: verify (the TAS to 9,000 frames, states at 2,345, 4,500, 7,200), tas (XMen2Native,
the TAS's stepped script to 15,000), session (the TAS by runner frame in the session machine at
16:9, to 8,000), mp (scene 1 with four players in the session machine, the framebuffers too, to its
4,433 frames). A section counts as caught when a configuration catches it at a frame tried.
Exits 1 when a load differs from its run or another layout loads."""
import argparse
import json
import os
import re
import secrets
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402
import harness  # noqa: E402
import xmen2check  # noqa: E402

NATIVE_SECTIONS = ["fiber", "arena", "players34", "machine", "platform", "sound", "snd2", "snd_music", "snd_parts",
                   "p34video"]
ENGINE_SECTIONS = ["exec", "cpu", "sched", "exact", "ram", "machine", "fm", "psg", "evq", "input", "game", "testing"]


def configs(refs: Path, out: Path) -> dict:
    """name: (runner, script, args, total frames, state frames, later frame for the section hashes, framebuffers)"""
    out.mkdir(parents=True, exist_ok=True)
    tas_stepped = out / "tas-stepped.input"
    xmen2check.write_script(xmen2check.stepped_pads(refs / "tas.input", refs / "tas-recomp-sync"), tas_stepped)
    tas_frames = out / "tas.input"   # by runner frame, as the verify runner and a played seat take it
    tas_frames.write_text("".join(l for l in (refs / "tas.input").read_text().splitlines(True)
                                  if not l.strip().upper().startswith("EXIT")), newline="\n")
    return {
        "verify": ("verify", tas_frames, [], 9000, [2345, 4500, 7200], 0, False),
        "tas": ("native", tas_stepped, ["--no-render"], 15000, [3000, 7200, 12600], 14000, False),
        "session": ("native", tas_frames, ["--session", "1", "--options", "widescreen=16:9"], 8000, [2400, 4800, 7200],
                    7800, False),
        "mp": ("native", common.GAME / "mp_inputs" / "scene1.input",
               ["--session", "1", "--scene", "1", "--options", "players=4"], 4433, [1200, 2400, 3600], 4300, True),
    }


def check(name: str, cfg: tuple, out: Path, insns: Path) -> dict:
    kind, script, extra, total, frames, later, fb = cfg
    src = xmen2check.exe_path(xmen2check.NATIVE_EXE if kind == "native" else xmen2check.VERIFY_EXE)
    if not src.is_file():
        sys.exit(f"no runner at {src}: run `xmen2check.py build`")
    cookie = secrets.token_hex(6)
    out.mkdir(parents=True, exist_ok=True)

    def run(tag: str, more: list[str], env_more: dict | None = None, own_exe: bool = False) -> dict:
        d = out / tag
        d.mkdir(parents=True, exist_ok=True)
        rd = d / "runner"
        rd.mkdir(exist_ok=True)
        xmen2check.copy_runner(src, rd)
        env = {**os.environ, "SDL_AUDIODRIVER": "dummy", "GENESIS_NO_LAUNCHER": "1", "XMEN2_GS_COOKIE": cookie,
               **xmen2check.crash_env(src), **(env_more or {})}
        args = list(extra)
        if kind == "native":
            env.update(XMEN2_SYNC_CSV=str(d / "sync.csv"), XMEN2_STATE_SECTIONS="1")
            args += ["--digest", str(d / "digest.csv")]
            if fb:
                args += ["--hash-frames", "1", "--hash-out", str(d / "fbhash.log")]
            if later and tag != "saved":
                (d / "later.txt").write_text(f"{later} {d / 'later.state'}\n")
                args += ["--state-at", str(d / "later.txt")]
        else:
            env.update(XMEN2_VERIFY_OUT=str(d / "verify.json"), XMEN2_VERIFY_SYNTH=str(common.GAME / "verify"),
                       XMEN2_VERIFY_INSNS=str(insns))
            args += ["--fresh-evidence"]
        exe = src if own_exe else rd / src.name
        r = subprocess.run([str(exe), str(common.ROM), "--benchmark", str(total), "--input-script", str(script), *args,
                            *more], cwd=rd, env=env, capture_output=True, text=True, errors="replace", timeout=3600)
        (d / "runner.log").write_text(r.stderr, encoding="utf-8")
        m = re.search(r"GENESISRECOMP_BENCHMARK (\{.*\})", r.stdout)
        c = re.search(r"\[xmen2\] counters (\{.*\})", r.stderr)
        counters = json.loads(c.group(1)) if c else None
        if counters and "state" in counters:
            counters["state"].pop("fiber_stack_peak", None)   # the heap's history (NOTES "Native traces branch")
        s = re.search(rf"\[state\] sections {later} (.*)", r.stderr) if later else None
        sections = dict(x.split("=", 1) for x in s.group(1).split()) if s else {}
        sections.pop("fiber", None)   # its raw stack: below the live frames, each process's own leftovers
        files = {f: (d / f).read_bytes() for f in ("sync.csv", "digest.csv", "fbhash.log", "verify.json") if (d / f).exists()}
        why = [l for l in r.stderr.splitlines() if l.startswith(("[state]", "[xmen2] state", "[crash]"))]
        return {"code": r.returncode, "files": files, "counters": counters, "sections": sections,
                "hash": (json.loads(m.group(1)) if m else {}).get("state_fnv1a64"), "said": why[-1] if why else ""}

    def differ(a: dict, b: dict) -> list[str]:
        d = [f"exit {b['code']}"] if a["code"] != b["code"] else []
        d += [f for f in sorted(set(a["files"]) | set(b["files"])) if a["files"].get(f) != b["files"].get(f)]
        d += ["counters"] if a["counters"] != b["counters"] else []
        d += ["state hash"] if a["hash"] != b["hash"] else []
        bad = sorted(k for k in set(a["sections"]) | set(b["sections"]) if a["sections"].get(k) != b["sections"].get(k))
        return d + ([f"sections at {later}: {'/'.join(bad)}"] if bad else [])

    plan = out / "states.txt"
    plan.write_text("".join(f"{f} {out / f'{f}.state'}\n" for f in frames)
                    + (f"{later} {out / 'later.state'}\n" if later else ""), newline="\n")
    saved = run("saved", ["--state-at", str(plan)])
    res = {"frames": frames, "saved_exit": saved["code"], "loads": {}, "left_out": {}, "layout": {}}
    print(f"{name}: {kind}, {total} frames, states at {frames}: exit {saved['code']}", flush=True)
    for f in frames:
        st = out / f"{f}.state"
        d = differ(saved, run(f"load-{f}", ["--state-load", str(st)]))
        res["loads"][f] = d
        print(f"  load at {f}: {'the same as the run that saved it' if not d else 'DIFFERS: ' + ', '.join(d)}", flush=True)
        for sec in NATIVE_SECTIONS if kind == "native" else ENGINE_SECTIONS:
            if kind == "native":
                r = run(f"skip-{f}-{sec}", ["--state-load", str(st), "--state-skip", sec])
            elif sec == "testing":
                r = run(f"skip-{f}-{sec}", ["--state-load", str(st)], {"XMEN2_STATE_SKIP_TESTING": "1"})
            else:
                r = run(f"skip-{f}-{sec}", ["--state-load", str(st)], {"GENESIS_RB_NEGCTL_SKIP": sec})
            d = differ(saved, r)
            res["left_out"].setdefault(sec, {})[f] = d
            print(f"    {sec} left out: {'caught (' + ', '.join(d) + ')' if d else 'not caught'}", flush=True)
    st = out / f"{frames[-1]}.state"
    for tag, more, env, own in (("other-cookie", [], {"XMEN2_GS_COOKIE": secrets.token_hex(6)}, False),
                                ("aslr-runner", [], None, True),
                                ("other-cookie-unchecked", [], {"XMEN2_GS_COOKIE": secrets.token_hex(6),
                                                                "XMEN2_STATE_PLACE_UNCHECKED": "1"}, False)):
        r = run(tag, ["--state-load", str(st), *more], env, own)
        res["layout"][tag] = {"exit": r["code"], "differs": differ(saved, r), "said": r["said"]}
        print(f"  {tag}: exit {r['code']}: {r['said'][:160]}", flush=True)
    return res


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--only", default="verify,tas,session,mp")
    ap.add_argument("--refs", default="", help="phase.py's build/phase_traces (default: $XMEN2_REFS, harness.py's)")
    ap.add_argument("--out", default="")
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args()
    common.ensure_rom()
    out = Path(a.out).resolve() if a.out else common.BUILD / "statecheck"
    refs = harness.refs_dir(a.refs)
    cfgs = configs(refs, out)
    insns = out / "insns.txt"
    insns.write_text("".join(f"{x} {len(p)}\n" for x, p in sorted(xmen2check.code_addrs_by_function().items())))
    results = {n: check(n, cfgs[n], out / n, insns) for n in a.only.split(",")}
    caught = {}
    for n, r in results.items():
        for sec, by in r["left_out"].items():
            caught.setdefault(sec, [])
            if any(by.values()):   # as phase.py's M2: a section that holds nothing yet at some frame is not missed there
                caught[sec].append(n)
    summary = {"loads_identical": all(not d for r in results.values() for d in r["loads"].values()),
               "other_layouts_refused": all(r["layout"][t]["exit"] != 0 for r in results.values()
                                            for t in ("other-cookie", "aslr-runner")),
               "caught": {s: v for s, v in caught.items() if v}, "not_caught": sorted(s for s, v in caught.items() if not v),
               "configs": results}
    (out / "statecheck.json").write_text(json.dumps(summary, indent=1))
    print(f"loads identical: {summary['loads_identical']}; other layouts refused: {summary['other_layouts_refused']}; "
          f"sections caught: {len(summary['caught'])}, not caught: {', '.join(summary['not_caught']) or 'none'}")
    if a.json:
        print(json.dumps({k: v for k, v in summary.items() if k != "configs"}))
    return 0 if summary["loads_identical"] and summary["other_layouts_refused"] else 1


if __name__ == "__main__":
    sys.exit(main())
