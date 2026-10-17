"""The per-batch gate a phase-5 worker runs in its own worktree and build directory (xmen2disasm
docs/LESSONS.md: "Make a ~6,000-frame TAS replay plus verify part of every batch gate"):

    uv run tools/gate.py [--files game/c/A.c ...] [--mode handc|native] [--frames 6000] [--base origin/main]

The files default to what changed against the merge base with --base (commits, uncommitted and
untracked). Steps, independent ones at once, fail-fast (the first failure stops the others), one
line each, then a one-line verdict; exit 1 on any failure:

  build    `xmen2check.py build` (this worktree's XMEN2_BUILD, default build/)
  pccheck  the changed game/c files compile as C++ (all of them when a header changed)
  mpw      every changed game/c file that byte-matched at the end of phase 4
           (docs/p4_mpw_matched.txt; all of those when a header changed) still matches:
           xmen2disasm `uv run tools/cc.py --match FILE` in $XMEN2_DISASM
  verify   shadow tests of the changed functions (every function when anything but game/c,
           game/verify or docs changed) on the TAS and four short corpus runs, their first
           --frames frames; fails on a mismatch or a run whose testing stopped early
  trace    the TAS's first --frames sync points in --mode against this build's stepped-gen rows
           (made once per XMen2SteppedGen hash and cached: tools/harness.py)

Corpus inputs: --refs or $XMEN2_REFS (tools/harness.py).
"""
from __future__ import annotations

import argparse
import concurrent.futures
import json
import os
import re
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402
import harness  # noqa: E402

GATE_RUNS = ("tas", "scenario-two-player", "scenario-pause-cheats", "scenario-options-menu",
             "scenario-char-nightcrawler")
MATCHED = common.ROOT / "docs" / "p4_mpw_matched.txt"
# changes that leave the runners' code alone (else verify tests every function)
NOT_CODE = re.compile(r"^(docs/|game/c/[0-9A-Fa-f]{6}\.c$|game/verify/|\.claude/|README|CLAUDE\.md$|[^/]*\.md$|"
                      r"tools/(?!bridge\.py$)[^/]*\.py$|\.gitignore$)")
TOOL_ERROR = re.compile(r"Unable to open the resource fork|end-of-file reached during read operation")


def git(*args: str) -> list[str]:
    import subprocess
    r = subprocess.run(["git", "-C", str(common.ROOT), *args], capture_output=True, text=True)
    return [l.strip() for l in r.stdout.splitlines() if l.strip()]


def changed_files(base: str) -> list[str]:
    mb = (git("merge-base", base, "HEAD") or [base])[0]
    files = set(git("diff", "--name-only", mb, "HEAD")) | set(git("diff", "--name-only", "HEAD"))
    files |= set(git("ls-files", "--others", "--exclude-standard"))
    return sorted(f.replace("\\", "/") for f in files)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--files", nargs="*", default=None, help="the batch's files (default: changed vs --base)")
    ap.add_argument("--base", default="origin/main")
    ap.add_argument("--mode", default="", help="trace mode: handc or native (default: native if the target exists)")
    ap.add_argument("--frames", type=int, default=6000, help="frames of each verify run / sync points of the trace")
    ap.add_argument("--refs", default="", help="corpus inputs (default $XMEN2_REFS)")
    a = ap.parse_args()
    t_start = time.time()
    common.ensure_rom()
    refs = harness.refs_dir(a.refs)
    mode = a.mode or ("native" if harness.native_supported() else "handc")
    def rel(f: str) -> str:   # a path given from anywhere -> relative to this worktree
        p = Path(f)
        if not p.is_absolute() and not (common.ROOT / p).exists() and p.exists():
            p = p.resolve()
        return p.resolve().relative_to(common.ROOT.resolve()).as_posix() if p.is_absolute() else p.as_posix()

    files = changed_files(a.base) if a.files is None else [rel(f) for f in a.files]
    cfiles = [f for f in files if re.fullmatch(r"game/c/[0-9A-Fa-f]{6}\.c", f) and (common.ROOT / f).is_file()]
    headers = [f for f in files if f.startswith("game/include/")]
    syn = [Path(f).stem.upper() for f in files if re.fullmatch(r"game/verify/[0-9A-Fa-f]{6}\.syn", f)]
    code_other = [f for f in files if not NOT_CODE.match(f) and not f.startswith("game/include/")]
    verify_all = bool(headers or code_other or not files)
    matched = {l.strip().upper() for l in MATCHED.read_text(encoding="utf-8").splitlines()
               if l.strip() and not l.startswith("#")}
    mpw_files = (sorted(f"game/c/{x}.c" for x in matched if (common.ROOT / f"game/c/{x}.c").is_file()) if headers
                 else [f for f in cfiles if Path(f).stem.upper() in matched])
    funcs = sorted({Path(f).stem.upper() for f in cfiles} | set(syn))
    runs = [r for r in GATE_RUNS if (refs / "inputs" / f"{r}.input").is_file()]
    work = common.BUILD / "gate"
    work.mkdir(parents=True, exist_ok=True)
    print(f"gate: {len(files)} changed files ({len(cfiles)} game/c, {len(headers)} headers, {len(code_other)} other code); "
          f"mode {mode}; build {common.BUILD}", flush=True)

    procs = harness.Procs()
    env = {k: v for k, v in os.environ.items() if k != "VIRTUAL_ENV"}

    def step_build():   # all five runners (XMen2Native too; `--native` would only add the scan)
        rc, so, se = procs.run(harness.check_cmd("build", "--json"),
                               env={**os.environ, "MSBUILDDISABLENODEREUSE": "1"})
        j = harness.last_json(so) or {}
        if rc != 0 or not j.get("ok"):
            return False, (se.strip().splitlines() or [f"build failed, see {j.get('log')}"])[-1][:200]
        return True, "runners built"

    def step_pccheck():
        if not cfiles and not headers:
            return True, "skipped: no changed C or header"
        lst = work / "pccheck_files.txt"
        lst.write_text("\n".join(str(common.ROOT / f) for f in cfiles) + "\n", encoding="utf-8")
        args = ["--all"] if headers else ["--files-from", str(lst)]
        rc, so, se = procs.run([sys.executable, str(common.ROOT / "tools" / "pccheck.py"), *args])
        lines = so.strip().splitlines()
        if rc != 0:
            bad = [l for l in lines if l.endswith(": FAILED") or "not named" in l]
            return False, (lines[-1] if lines else se.strip()[-200:]) + (f"; first: {bad[0]}" if bad else "")
        return True, lines[-1] if lines else "ok"

    def step_mpw():
        if not mpw_files:
            return True, f"skipped: none of the {len(cfiles)} changed files byte-matched at the end of phase 4"
        cc = common.DISASM_MAIN / "tools" / "cc.py"
        if not cc.is_file():
            return False, f"no {cc}: set XMEN2_DISASM to the xmen2disasm checkout"

        def one(f: str):
            for _attempt in range(3):
                rc, so, se = procs.run(["uv", "run", "tools/cc.py", "--match", str(common.ROOT / f), "-q"],
                                       env=env, cwd=common.DISASM_MAIN, timeout=600)
                if rc == 0 or rc == -1 or not TOOL_ERROR.search(so + se):
                    break
            return f, rc, (so.strip().splitlines() or [se.strip()[-200:]])[0]

        with concurrent.futures.ThreadPoolExecutor(8) as ex:
            got = list(ex.map(one, mpw_files))
        bad = [(f, line) for f, rc, line in got if rc != 0]
        if bad:
            return False, f"{len(bad)}/{len(got)} no longer match; first: {bad[0][1][:160]}"
        return True, f"{len(got)}/{len(got)} still byte-match" + (" (a header changed: every matched file)" if headers else "")

    def step_verify():
        args = ["verify", "--inputs", str(refs / "inputs"), "--runs", ",".join(runs), "--frames", str(a.frames),
                "--jobs", str(len(runs)), "--out", str(work / "verify"), "--json"]
        if not verify_all:
            if not funcs:
                return True, "skipped: no function changed"
            args += ["--functions", ",".join(funcs)]
        rc, so, se = procs.run(harness.check_cmd(*args))
        j = harness.last_json(so)
        if rc != 0 or j is None:
            return False, (se.strip().splitlines() or ["verify failed"])[-1][:240]
        fs = j.get("functions", {})
        mism = {k: v for k, v in fs.items() if v.get("mismatches")}
        if mism:
            k, v = next(iter(sorted(mism.items())))
            return False, f"{len(mism)} functions mismatch; first ${k}: {str(v.get('first_mismatch'))[:180]}"
        want = set(fs) if verify_all else set(funcs)
        tested = {k for k in want if fs.get(k, {}).get("calls_checked")}
        unreached = sorted(want - tested)
        calls = sum(fs.get(k, {}).get("calls_checked", 0) for k in want)
        return True, (f"{len(tested)}/{len(want)} functions checked ({calls} calls, 0 mismatches) on {len(runs)} runs x "
                      f"{a.frames} frames" + (f"; not reached: {', '.join(unreached[:6])}{' ...' if len(unreached) > 6 else ''}"
                                              if unreached and not verify_all else ""))

    def step_trace():
        ref = harness.stepped_gen_ref(procs, refs, "tas", frames=a.frames, work=work)
        if not ref["ok"]:
            return False, ref["detail"]
        g = harness.trace(procs, refs, "tas", mode, work / f"trace-tas-{mode}", ref["csv"], frames=a.frames)
        return g["ok"], f"tas, {g['detail']} (reference rows {'made' if ref['made'] else 'cached'})"

    steps = {"build": step_build, "pccheck": step_pccheck, "mpw": step_mpw}
    after_build = {"verify": step_verify, "trace": step_trace}
    order = ["build", "pccheck", "mpw", "verify", "trace"]
    failed: list[str] = []
    printed: set[str] = set()
    started: dict[str, float] = {}

    def report(name: str, ok: bool, detail: str) -> None:
        printed.add(name)
        dt = time.time() - started.get(name, t_start)
        print(f"{name:8} {'PASS' if ok else 'FAIL'} {dt:6.1f} s  {detail}", flush=True)

    with concurrent.futures.ThreadPoolExecutor(5) as ex:
        futs = {}
        for n, fn in steps.items():
            started[n] = time.time()
            futs[ex.submit(fn)] = n
        while futs:
            done, _ = concurrent.futures.wait(futs, return_when=concurrent.futures.FIRST_COMPLETED)
            for f in done:
                n = futs.pop(f)
                try:
                    ok, detail = f.result()
                except Exception as e:   # a step's own error is a failure, not a crash of the gate
                    ok, detail = False, f"{type(e).__name__}: {e}"
                if not ok and not failed:
                    failed.append(n)
                    procs.stop_all()
                    report(n, ok, detail)
                elif ok:
                    report(n, ok, detail)
                if n == "build" and ok and not failed:
                    for m, fn in after_build.items():
                        started[m] = time.time()
                        futs[ex.submit(fn)] = m
    for n in order:
        if n not in printed:
            print(f"{n:8} ---- {'stopped' if n in started else 'not run'} (the gate failed at {failed[0]})", flush=True)
    total = time.time() - t_start
    if failed:
        print(f"gate: FAIL at {failed[0]} ({total:.0f} s)")
        return 1
    print(f"gate: PASS ({total:.0f} s)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
