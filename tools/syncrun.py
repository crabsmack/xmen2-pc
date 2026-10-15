"""Phase-4 sync loop (dev): timed `--sync` trace and stepped replay of corpus runs, compared.

    uv run tools/syncrun.py tas                  # one run: D1 trace, D2 stepped, compare
    uv run tools/syncrun.py --all [--timed-only]  # every corpus run; prints a7_min per run
    uv run tools/syncrun.py --all --stepped-only  # reuse the timed traces

Inputs come from xmen2disasm `emu/export_input.py`; outputs land in build/syncrun/<run>/{timed,stepped}.
The contract is still `uv run tools/phase.py 4` in xmen2disasm; this only shortens the loop.
"""
from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

OUT = common.BUILD / "syncrun"


def corpus_runs() -> list[str]:
    r = subprocess.run(["uv", "run", "python", "-c",
                        "import sys; sys.path.insert(0, 'emu'); import corpus; "
                        "print('\\n'.join(n for n, _ in corpus.corpus()))"],
                       cwd=common.DISASM_MAIN, capture_output=True, text=True)
    return [l for l in r.stdout.split() if l]


def contract(*args: str) -> dict | None:
    r = subprocess.run(["uv", "run", "tools/xmen2check.py", *args, "--json"], cwd=common.ROOT,
                       capture_output=True, text=True)
    i = r.stdout.find("{")
    if r.returncode or i < 0:
        print(f"  FAILED: {(r.stdout + r.stderr).strip()[-300:]}")
        return None
    return json.loads(r.stdout[i:])


def one(run: str, timed: bool, stepped: bool) -> dict:
    d = OUT / run
    d.mkdir(parents=True, exist_ok=True)
    res = {"run": run}
    if timed:
        inp = d / f"{run}.input"
        if not inp.exists():
            subprocess.run(["uv", "run", "emu/export_input.py", run, "--out", str(inp)], cwd=common.DISASM_MAIN,
                           capture_output=True, text=True)
        t = contract("trace", "--run", run, "--input", str(inp), "--out", str(d / "timed"), "--sync")
        res["timed"] = t
        if t:
            cmp = subprocess.run(["uv", "run", "tools/compare.py", str(d / "timed" / "frames.csv"), "--ref", run],
                                 cwd=common.ROOT, capture_output=True, text=True)
            res["frames_equal"] = cmp.returncode == 0
    if stepped:
        s = contract("trace", "--mode", "stepped", "--run", run, "--replay", str(d / "timed"),
                     "--out", str(d / "stepped"))
        res["stepped"] = s
        if s:
            cmp = subprocess.run(["uv", "run", "tools/synccmp.py", str(d / "timed" / "sync.csv"),
                                  str(d / "stepped" / "sync.csv")], cwd=common.ROOT, capture_output=True, text=True)
            res["sync_equal"] = cmp.returncode == 0
            res["sync_detail"] = cmp.stdout.strip().splitlines()
    return res


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("runs", nargs="*")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--timed-only", action="store_true")
    ap.add_argument("--stepped-only", action="store_true")
    a = ap.parse_args()
    runs = corpus_runs() if a.all else a.runs
    results = []
    for run in runs:
        r = one(run, not a.stepped_only, not a.timed_only)
        t, s = r.get("timed") or {}, r.get("stepped") or {}
        line = f"{run:28} "
        if not a.stepped_only:
            line += f"timed frames_equal={r.get('frames_equal')} syncs={t.get('sync_points')} a7_min={t.get('a7_min')} "
        if not a.timed_only:
            line += f"stepped syncs={s.get('sync_points')} equal={r.get('sync_equal')}"
            if r.get("sync_equal") is False:
                line += " | " + "; ".join(x for x in r["sync_detail"] if "None" not in x)
        print(line, flush=True)
        results.append(r)
    (OUT / "last.json").write_text(json.dumps(results, indent=1))
    a7 = [r["timed"]["a7_min"] for r in results if r.get("timed") and r["timed"].get("a7_min") is not None]
    if a7:
        print(f"lowest A7 over these runs: ${min(a7):06X}")
    bad = [r["run"] for r in results if r.get("frames_equal") is False or r.get("sync_equal") is False
           or (not a.stepped_only and not r.get("timed")) or (not a.timed_only and not r.get("stepped"))]
    print(f"{len(results) - len(bad)}/{len(results)} ok" + (f"; first bad: {bad[0]}" if bad else ""))
    return 1 if bad else 0


if __name__ == "__main__":
    raise SystemExit(main())
