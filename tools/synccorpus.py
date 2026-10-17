"""Run a stepped-family trace mode over a whole corpus of phase-4 references and compare every
run's sync rows with its stepped-gen reference (P4 D2/D6, P5 P2): every row, `sync` and the four
CRCs, the same number of rows. Runs in parallel, longest first.

    uv run tools/synccorpus.py --refs REFS --mode native --out DIR [--jobs 20] [--runs a,b]
                               [--keep-runner] [--env NAME=VALUE ...] [--json]

REFS holds, per run R (what xmen2disasm's phase.py 4 leaves in build/phase_traces):
    R.input                 the run's exported input
    R-recomp-sync/          its D1 trace (the REFDIR of --replay)
    R-stepped-gen/sync.csv  the reference rows
Each run is `xmen2check.py trace --mode MODE --replay REFS/R-recomp-sync --run R --input
REFS/R.input --out DIR/R --json`; its runner copy (DIR/R/runner, ~70 MB) is deleted afterwards
unless --keep-runner. Writes DIR/summary.json; exit 0 only if every run is equal.
"""
from __future__ import annotations

import argparse
import concurrent.futures
import csv
import json
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

COLS = ("ram_crc32", "vram_crc32", "cram_crc32", "vsram_crc32")


def rows(p: Path) -> list[tuple]:
    with open(p, newline="") as f:
        return [(int(r["sync"]), *(r[c] for c in COLS)) for r in csv.DictReader(f)]


def compare(ref: Path, got: Path) -> dict:
    """Every row in order: sync index and the four CRCs; the same count."""
    a, b = rows(ref), rows(got)
    first = next((i for i, (x, y) in enumerate(zip(a, b)) if x != y), None)
    res = {"ref_rows": len(a), "rows": len(b), "equal": first is None and len(a) == len(b)}
    if first is not None:
        res["first_diff"] = {"row": first, "sync": a[first][0],
                             "columns": [c for c, x, y in zip(COLS, a[first][1:], b[first][1:]) if x != y]
                             or ["sync"]}
    elif len(a) != len(b):
        res["first_diff"] = {"row": min(len(a), len(b)), "sync": None, "columns": ["row count"]}
    return res


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--refs", required=True, type=Path)
    ap.add_argument("--mode", default="native")
    ap.add_argument("--out", required=True, type=Path)
    ap.add_argument("--jobs", type=int, default=20)
    ap.add_argument("--runs", default="", help="only these (comma-separated)")
    ap.add_argument("--keep-runner", action="store_true")
    ap.add_argument("--env", action="append", default=[], help="NAME=VALUE for the runner (e.g. a self-test)")
    ap.add_argument("--timeout", type=int, default=7200)
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args()
    refs = a.refs.resolve()
    runs = sorted(p.stem for p in refs.glob("*.input"))
    if a.runs:
        keep = set(a.runs.split(","))
        runs = [r for r in runs if r in keep]
    runs = [r for r in runs if (refs / f"{r}-recomp-sync" / "sync.csv").is_file()
            and (refs / f"{r}-stepped-gen" / "sync.csv").is_file()]
    if not runs:
        sys.exit(f"no complete references in {refs}")
    a.out.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ)
    for kv in a.env:
        k, _, v = kv.partition("=")
        env[k] = v
    size = {r: sum(1 for _ in open(refs / f"{r}-stepped-gen" / "sync.csv")) for r in runs}

    def one(r: str) -> tuple[str, dict]:
        t0 = time.time()
        out = a.out / r
        p = subprocess.run([sys.executable, str(common.ROOT / "tools" / "xmen2check.py"), "trace", "--mode", a.mode,
                            "--replay", str(refs / f"{r}-recomp-sync"), "--run", r, "--input", str(refs / f"{r}.input"),
                            "--out", str(out), "--json", "--timeout", str(a.timeout)],
                           capture_output=True, text=True, env=env)
        res: dict = {"seconds": round(time.time() - t0, 1)}
        if p.returncode:
            res.update(equal=False, error=(p.stdout + p.stderr).strip()[-600:])
        else:
            j = json.loads(p.stdout.strip().splitlines()[-1])
            res.update({k: j[k] for k in ("sync_points", "dispatch_misses", "interp_instructions", "fps") if k in j})
            if "exported" in j:
                res["exported"] = j["exported"]
            res.update(compare(refs / f"{r}-stepped-gen" / "sync.csv", out / "sync.csv"))
            res["equal"] = res["equal"] and j.get("dispatch_misses") == 0 and j.get("interp_instructions") == 0
        if not a.keep_runner and (out / "runner").exists():
            shutil.rmtree(out / "runner", ignore_errors=True)
        print(f"{r}: {'EQUAL' if res['equal'] else 'DIFFERS'} ({res.get('sync_points', '?')} sync points, "
              f"{res['seconds']} s){'' if res['equal'] else ' ' + json.dumps(res.get('first_diff') or res.get('error'))}",
              file=sys.stderr, flush=True)
        return r, res

    t0 = time.time()
    order = sorted(runs, key=lambda r: -size[r])   # longest first: the last to finish are short
    with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, min(a.jobs, len(runs)))) as ex:
        results = dict(ex.map(one, order))
    per = {r: results[r] for r in runs}
    summary = {"mode": a.mode, "runs": len(runs), "equal": sum(v["equal"] for v in per.values()),
               "failed": [r for r, v in per.items() if not v["equal"]], "wall_seconds": round(time.time() - t0, 1),
               "jobs": a.jobs, "env": a.env, "per_run": per}
    (a.out / "summary.json").write_text(json.dumps(summary, indent=1), encoding="utf-8")
    brief = {k: v for k, v in summary.items() if k != "per_run"}
    print(json.dumps(summary if a.json else brief, indent=None if a.json else 1))
    return 0 if summary["equal"] == summary["runs"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
