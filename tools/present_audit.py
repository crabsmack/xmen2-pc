"""What high_fps's in-between frames leave still: XMEN2_PRESENT_AUDIT (platform/native/present.cpp,
NOTES "high_fps coverage").

    uv run tools/present_audit.py run OUT [--runs tas,scenario-...] [--jobs N] [--options high_fps=144]
    uv run tools/present_audit.py summary OUT/*/audit.csv [--top N]

`run` plays the TAS and the scenario inputs ($XMEN2_REFS) natively with the audit on, each into
OUT/<run>/ (audit.csv, trace/, trace.log), and prints each run's `present` counters. `summary` ranks
the audit's rows: U (sprites left where R_k has them) by reason and owner class, with how many of an
instance's 'new' frames come every other frame (a blink); A (instances whose pieces move unlike
their anchor); P (blocks of plane cells moved by rewriting the nametable) by run and frame range.
An actor's class is its descriptor's ROM address (game/include/xmen2_data.h names them: Tbl_*_vtable).
"""
from __future__ import annotations

import argparse
import concurrent.futures
import json
import os
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
WHY = {"1": "no_owner", "2": "new", "3": "size", "4": "snap"}


def run(a: argparse.Namespace) -> None:
    sys.path.insert(0, str(ROOT / "tools"))
    import harness  # noqa: E402  (the corpus's place: --refs, $XMEN2_REFS)
    refs = harness.refs_dir(a.refs)
    runs = a.runs.split(",") if a.runs else ["tas"] + sorted(p.stem for p in refs.glob("scenario-*.input"))
    out = Path(a.out).resolve()
    out.mkdir(parents=True, exist_ok=True)

    def one(name: str) -> str:
        d = out / name
        d.mkdir(exist_ok=True)
        env = dict(os.environ, XMEN2_PRESENT_AUDIT=str(d / "audit.csv"))
        cmd = ["uv", "run", "tools/xmen2check.py", "trace", "--mode", "native", "--run", name,
               "--input", str(refs / f"{name}.input"), "--out", str(d / "trace"), "--options", a.options, "--json"]
        r = subprocess.run(cmd, cwd=ROOT, env=env, capture_output=True, text=True)
        (d / "trace.log").write_text(r.stdout + r.stderr)
        try:
            pres = json.loads(r.stdout[r.stdout.index("{"):]).get("enhance", {}).get("present", {})
        except ValueError:
            pres = {}
        return f"{name}: exit {r.returncode} {json.dumps(pres)}"

    with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:
        for line in ex.map(one, runs):
            print(line, flush=True)


def summary(a: argparse.Namespace) -> None:
    u, u_frames, u_ex, alt = Counter(), defaultdict(set), {}, Counter()
    an, an_ex = Counter(), {}
    planes: dict[str, list] = defaultdict(list)
    totals = Counter()
    for path in a.files:
        name = Path(path).parent.name
        seen = defaultdict(set)
        for line in open(path):
            f = line.rstrip("\n").split(",")
            totals[f[0]] += 1
            if f[0] == "U":
                key = (WHY.get(f[4], f[4]), f[10])
                u[key] += 1
                u_frames[key].add((name, int(f[1])))
                u_ex.setdefault(key, (name, int(f[1])))
                if f[4] == "2":
                    seen[(f[10], f[11])].add(int(f[1]))
            elif f[0] == "A":
                key = (f[6], f[4], int(f[9]) - int(f[7]), int(f[10]) - int(f[8]))
                an[key] += 1
                an_ex.setdefault(key, (name, int(f[1])))
            elif f[0] == "P":
                planes[f"{name} plane {f[2]}"].append((int(f[1]), int(f[3]), int(f[4]), int(f[5]), int(f[6])))
        for (cls, _), fs in seen.items():
            alt[("new", cls)] += sum(1 for k in fs if k - 2 in fs and k - 1 not in fs)
    print("rows:", dict(totals))
    print(f"\nU: sprites left still, by (reason, owner class): top {a.top} of {len(u)}")
    for key, n in u.most_common(a.top):
        blink = f", every other frame {alt[key]}" if alt.get(key) else ""
        print(f"  {n:7d} rows {len(u_frames[key]):6d} frames  {key[0]:8s} {key[1]}  e.g. {u_ex[key]}{blink}")
    print(f"\nA: instances whose pieces move unlike their anchor, (class, def, step - anchor step): {len(an)}")
    for key, n in an.most_common(a.top):
        print(f"  {n:7d} frames  {key}  e.g. {an_ex[key]}")
    print(f"\nP: blocks of plane cells moved, frame ranges (gaps over 30 frames split them)")
    for key, rows in sorted(planes.items()):
        groups: list[list] = []
        for r in rows:
            if groups and r[0] - groups[-1][-1][0] <= 30:
                groups[-1].append(r)
            else:
                groups.append([r])
        print(f"  {key}: {len(rows)} frames in {len(groups)} ranges")
        for g in groups[:a.top]:
            steps = Counter((r[1], r[2]) for r in g).most_common(2)
            print(f"    {g[0][0]:6d}-{g[-1][0]:6d}  {len(g):4d} frames  votes/changed "
                  f"{sum(r[3] for r in g) / len(g):.0f}/{sum(r[4] for r in g) / len(g):.0f}  steps {steps}")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run")
    r.add_argument("out")
    r.add_argument("--runs", default="")
    r.add_argument("--jobs", type=int, default=8)
    r.add_argument("--options", default="high_fps=144")
    r.add_argument("--refs", default="")
    s = sub.add_parser("summary")
    s.add_argument("files", nargs="+")
    s.add_argument("--top", type=int, default=15)
    a = ap.parse_args()
    run(a) if a.cmd == "run" else summary(a)


if __name__ == "__main__":
    main()
