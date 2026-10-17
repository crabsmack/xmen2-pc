"""Measure the phase-5 P0 harness gates (xmen2disasm docs/LESSONS.md "Kickoff checklist") and
write docs/bench.json:

    uv run tools/bench.py [--quick] [--mode handc|native] [--refs DIR] [--jobs N] [--cached-refs]

  clean_build          a fresh XMEN2_BUILD directory, `xmen2check.py build` (all runners)  < 5 min
  corpus_check         every corpus run traced in the mode phase 5 uses (native once the
                       target exists, else handc) against this build's stepped-gen rows
                       (made again unless --cached-refs: tools/harness.py), plus a full
                       `verify`, all at once, longest first                                 < 15 min
  parallel_build_dirs  two fresh XMEN2_BUILD directories built at the same time: both succeed
                       and give byte-identical runners (workers each verify in their own)

--quick skips the three fresh builds (their gates are then null). The corpus check runs on this
worktree's build (XMEN2_BUILD or build/), built incrementally first. Inputs: --refs or
$XMEN2_REFS (harness.py). Run it at normal priority on an otherwise idle machine: the numbers
are the gates.
"""
from __future__ import annotations

import argparse
import concurrent.futures
import datetime
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
import harness  # noqa: E402

RUNNERS = ("XMen2Recomp.exe", "XMen2Stepped.exe", "XMen2SteppedGen.exe", "XMen2Verify.exe", "XMen2Native.exe")
GATES = {"clean_build": "< 5 min", "corpus_check": "< 15 min",
         "parallel_build_dirs": "two XMEN2_BUILD dirs built at once both succeed with identical runner hashes"}
LIMITS = {"clean_build": 300, "corpus_check": 900}


def log(msg: str) -> None:
    print(f"[bench {time.strftime('%H:%M:%S')}] {msg}", file=sys.stderr, flush=True)


def git(*args: str, cwd: Path = common.ROOT) -> str:
    return subprocess.run(["git", "-C", str(cwd), *args], capture_output=True, text=True).stdout.strip()


def build(build_dir: Path, extra: tuple[str, ...] = ()) -> dict:
    env = {**os.environ, "XMEN2_BUILD": str(build_dir), "MSBUILDDISABLENODEREUSE": "1"}
    t0 = time.time()
    r = subprocess.run(harness.check_cmd("build", "--json", *extra), env=env, capture_output=True, text=True)
    j = harness.last_json(r.stdout) or {}
    rel = build_dir / "Release"
    hashes = {n: common.sha1(rel / n) for n in RUNNERS if (rel / n).is_file()}
    return {"s": round(time.time() - t0, 1), "ok": r.returncode == 0 and j.get("ok") is True and len(hashes) == len(RUNNERS),
            "sha1": hashes, "detail": (r.stderr.strip().splitlines() or [""])[-1][:300] if r.returncode else ""}


# per-job speeds measured on this machine (NOTES.md "Phase 5 harness"), for longest-first only
VERIFY_FPS, TRACE_SYNC_PER_S = 200.0, 440.0


def corpus_check(refs: Path, mode: str, jobs: int, out: Path, fresh_refs: bool) -> dict:
    """Every verify run (`verify --runs R`: the same per-run test as one full verify) and every
    trace task (the run's stepped-gen reference rows, made unless cached, then the trace in
    `mode` compared with them) in one pool of `jobs`, longest first by estimated cost."""
    runs = harness.corpus_runs(refs)
    size = {r: harness.sync_count(refs, r) for r in runs}
    vruns = sorted(p.stem for p in (refs / "inputs").glob("*.input"))
    vframes = {r: sum(int(m.group(1), 0) for m in re.finditer(r"^WAIT\s+(\S+)", (refs / "inputs" / f"{r}.input").read_text(), re.M))
               for r in vruns}
    procs = harness.Procs()
    t0 = time.time()

    def verify_one(run: str) -> dict:
        v0 = time.time()
        rc, so, se = procs.run(harness.check_cmd("verify", "--inputs", str(refs / "inputs"), "--runs", run, "--jobs", "1",
                                                 "--out", str(out / "verify" / run), "--json"), timeout=6 * 3600)
        j = harness.last_json(so)
        funcs = (j or {}).get("functions") or {}
        bad = sum(1 for v in funcs.values() if v.get("mismatches"))
        per = ((j or {}).get("per_run") or {}).get(run) or {}
        if rc == 0 and j is not None:
            shutil.rmtree(out / "verify" / run, ignore_errors=True)
        return {"kind": "verify", "run": run, "s": round(time.time() - v0, 1), "end": time.time() - t0,
                "ok": rc == 0 and j is not None and bad == 0, "frames": vframes[run],
                "functions_tested": per.get("functions_tested"),
                "detail": (se.strip().splitlines() or [""])[-1][:300] if rc else f"{bad} functions with mismatches"}

    def trace_one(run: str) -> dict:
        ref = harness.stepped_gen_ref(procs, refs, run, work=out / "refs-work", fresh=fresh_refs)
        if not ref["ok"]:
            return {"kind": "trace", "run": run, "ok": False, "detail": ref["detail"], "s": 0.0, "ref_s": ref["s"],
                    "ref_made": ref["made"], "end": time.time() - t0}
        g = harness.trace(procs, refs, run, mode, out / "traces" / f"{run}-{mode}", ref["csv"])
        return {**g, "kind": "trace", "ref_s": ref["s"], "ref_made": ref["made"], "ref_fps": ref.get("fps"),
                "end": time.time() - t0}

    tasks = [(vframes[r] / VERIFY_FPS, verify_one, r) for r in vruns]
    tasks += [(size[r] * (1 if not fresh_refs else 2) / TRACE_SYNC_PER_S, trace_one, r) for r in runs]
    with concurrent.futures.ThreadPoolExecutor(jobs) as ex:
        got = list(ex.map(lambda t: t[1](t[2]), sorted(tasks, key=lambda t: -t[0])))
    total = round(time.time() - t0, 1)
    tr = [g for g in got if g["kind"] == "trace"]
    vf = [g for g in got if g["kind"] == "verify"]
    tbad, vbad = [g for g in tr if not g["ok"]], [g for g in vf if not g["ok"]]
    traces = {"s": round(max((g["end"] for g in tr), default=0), 1), "ok": not tbad and len(tr) == len(runs),
              "refs_made": sum(1 for g in tr if g["ref_made"]),
              "refs_job_s": round(sum(g["ref_s"] for g in tr), 1), "traces_job_s": round(sum(g["s"] for g in tr), 1),
              "detail": f"{len(tr) - len(tbad)}/{len(tr)} runs equal" + (f"; first: {tbad[0]['run']}: {tbad[0]['detail']}" if tbad else ""),
              "longest": sorted(({"run": g["run"], "s": g["s"], "ref_s": g["ref_s"], "sync_points": size[g["run"]],
                                  "fps": g.get("fps"), "ref_fps": g.get("ref_fps")} for g in tr),
                                key=lambda x: -(x["s"] + x["ref_s"]))[:6]}
    verify = {"s": round(max((g["end"] for g in vf), default=0), 1), "ok": not vbad and len(vf) == len(vruns),
              "job_s": round(sum(g["s"] for g in vf), 1),
              "detail": f"{len(vf) - len(vbad)}/{len(vf)} runs pass" + (f"; first: {vbad[0]['run']}: {vbad[0]['detail']}" if vbad else ""),
              "longest": sorted(({"run": g["run"], "s": g["s"], "frames": g["frames"],
                                  "fps": round(g["frames"] / g["s"]) if g["s"] else None} for g in vf),
                                key=lambda x: -x["s"])[:6]}
    return {"runs": len(runs), "verify_runs": len(vruns), "mode": mode, "jobs": jobs, "traces": traces,
            "verify": verify, "total_s": total}


def parallel_builds(dirs: list[Path]) -> list[dict]:
    with concurrent.futures.ThreadPoolExecutor(len(dirs)) as ex:
        return list(ex.map(build, dirs))


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--quick", action="store_true", help="skip the fresh builds (clean, parallel)")
    ap.add_argument("--mode", default="", help="trace mode (default: native if the target exists, else handc)")
    ap.add_argument("--refs", default="", help="phase-4 references (default $XMEN2_REFS)")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 8,
                    help="runners at once, verify runs and traces together (default: every thread)")
    ap.add_argument("--cached-refs", action="store_true",
                    help="reuse cached stepped-gen reference rows (default: make them again, the cost after an engine change)")
    ap.add_argument("--out", default=str(common.ROOT / "docs" / "bench.json"))
    ap.add_argument("--keep", action="store_true", help="keep the fresh build directories")
    a = ap.parse_args()
    common.ensure_rom()
    refs = harness.refs_dir(a.refs)
    mode = a.mode or ("native" if harness.native_supported() else "handc")
    dirty = bool(git("status", "--porcelain", "--", ".", ":(exclude)docs/bench.json"))
    res: dict = {"date": datetime.date.today().isoformat(),
                 "game_rev": git("rev-parse", "HEAD") + ("-dirty" if dirty else ""),
                 "engine_rev": git("rev-parse", "HEAD", cwd=common.ENGINE),
                 "machine": {"threads": os.cpu_count()}, "corpus_check_mode": mode, "quick": a.quick,
                 "priority": "normal", "gates": GATES}
    fresh = [common.ROOT / f"build-bench-{k}" for k in ("clean", "a", "b")]
    if not a.quick:
        for d in fresh:
            shutil.rmtree(d, ignore_errors=True)
        log("clean build (fresh XMEN2_BUILD, all runners)")
        c = build(fresh[0])
        res["clean_build_s"] = c["s"]
        res["clean_build"] = {k: c[k] for k in ("ok", "sha1", "detail")}
        log(f"clean build: {c['s']} s, ok {c['ok']}")
        log("two fresh builds at once")
        t0 = time.time()
        pa, pb = parallel_builds(fresh[1:])
        same = pa["ok"] and pb["ok"] and pa["sha1"] == pb["sha1"] == c["sha1"]
        res["parallel_build_dirs_ok"] = same
        res["parallel_builds"] = {"s": round(time.time() - t0, 1), "a": pa, "b": pb,
                                  "same_as_clean": pa["sha1"] == c["sha1"]}
        log(f"parallel builds: {res['parallel_builds']['s']} s, identical {same}")
        if not a.keep:
            for d in fresh:
                shutil.rmtree(d, ignore_errors=True)
    else:
        res.update(clean_build_s=None, parallel_build_dirs_ok=None)
    log("incremental build of this worktree's build dir")
    inc = build(common.BUILD)   # all five runners, XMen2Native included
    res["incremental_build_s"] = inc["s"]
    if not inc["ok"]:
        sys.exit(f"build failed: {inc['detail']}")
    res["runner_sha1"] = inc["sha1"]
    log(f"corpus check ({mode}, {a.jobs} jobs, {'cached' if a.cached_refs else 'fresh'} references)")
    out = common.BUILD / "bench"
    cc = corpus_check(refs, mode, a.jobs, out, fresh_refs=not a.cached_refs)
    res["corpus_check_s"] = {"traces_s": cc["traces"]["s"], "verify_s": cc["verify"]["s"], "total_s": cc["total_s"],
                             "mode": mode, "refs_made": cc["traces"]["refs_made"]}
    res["corpus_check"] = cc
    ok_cc = cc["traces"]["ok"] and cc["verify"]["ok"]
    res["pass"] = {"clean_build": None if res["clean_build_s"] is None else
                   res["clean_build"]["ok"] and res["clean_build_s"] < LIMITS["clean_build"],
                   "corpus_check": ok_cc and cc["total_s"] < LIMITS["corpus_check"],
                   "parallel_build_dirs": res["parallel_build_dirs_ok"]}
    Path(a.out).write_text(json.dumps(res, indent=1) + "\n", encoding="utf-8", newline="\n")
    print(json.dumps({k: res[k] for k in ("clean_build_s", "incremental_build_s", "corpus_check_s",
                                          "parallel_build_dirs_ok", "pass")}))
    return 0 if all(v is not False for v in res["pass"].values()) else 1


if __name__ == "__main__":
    raise SystemExit(main())
