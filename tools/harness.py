"""Shared by tools/bench.py and tools/gate.py: the corpus inputs, stepped-gen reference rows
cached by content, contract calls run as child processes that can be stopped (by their own PIDs,
never by image name), and the sync-point comparison phase.py makes (xmen2disasm
docs/phases/P4-decomp.md, D2/D6).

Inputs: $XMEN2_REFS, else $XMEN2_DISASM/build/phase_traces: the phase-4 trace directory phase.py
writes, with per run <run>.input, <run>-recomp-sync/ (the D1 timed trace: REFDIR for --replay,
the reference frame of each sync point) and inputs/<run>.input (for verify).

Reference rows: `trace --mode stepped-gen` (the frame-synchronous runner, all generated code) of
the run, made by THIS build's XMen2SteppedGen and cached under the git common directory
(.git/xmen2-cache/stepped-gen/<key>/sync.csv), keyed by the SHA-1 of that runner, the input and
the REFDIR sync.csv: shared by every worktree, and reused until one of the three changes (the
runners are reproducible: CMakeLists.txt XMEN2_REPRODUCIBLE).
"""
from __future__ import annotations

import csv
import hashlib
import json
import os
import shutil
import subprocess
import sys
import threading
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

CHECK = common.ROOT / "tools" / "xmen2check.py"
COMPARED = ("ram_crc32", "vram_crc32", "cram_crc32", "vsram_crc32")
COUNTERS = ("dispatch_misses", "interp_instructions", "stub_hits")
STEPPED_GEN = "XMen2SteppedGen.exe" if sys.platform == "win32" else "XMen2SteppedGen"


def refs_dir(arg: str = "") -> Path:
    d = Path(arg or os.environ.get("XMEN2_REFS") or common.DISASM_MAIN / "build" / "phase_traces")
    if not (d / "tas-recomp-sync" / "sync.csv").is_file() or not (d / "tas.input").is_file():
        sys.exit(f"no phase-4 corpus inputs in {d} (tas.input, tas-recomp-sync/sync.csv): set XMEN2_REFS to "
                 f"phase.py's build/phase_traces directory, or pass --refs")
    return d


def corpus_runs(refs: Path) -> list[str]:
    """Every run with an input and a D1 trace."""
    return sorted(d.name[:-len("-recomp-sync")] for d in refs.glob("*-recomp-sync")
                  if (d / "sync.csv").is_file() and (refs / (d.name[:-len("-recomp-sync")] + ".input")).is_file())


def sync_count(refs: Path, run: str) -> int:
    """The run's sync points (= frame-synchronous runner frames)."""
    with open(refs / f"{run}-recomp-sync" / "sync.csv", "rb") as f:
        return sum(1 for _ in f) - 1


def sync_rows(p: Path) -> dict[int, tuple[str, ...]]:
    with open(p, newline="") as f:
        return {int(r["sync"]): tuple(r[c].lower() for c in COMPARED) for r in csv.DictReader(f)}


def compare_sync(ref_csv: Path, got_csv: Path, limit: int | None = None) -> str | None:
    """None when every reference sync point (the first `limit`) is in the trace with the same
    CRCs and the trace has no others; else the first difference."""
    if not got_csv.is_file():
        return f"trace wrote no {got_csv}"
    ref, got = sync_rows(ref_csv), sync_rows(got_csv)
    want = sorted(s for s in ref if limit is None or s <= limit)
    for s in want:
        g = got.get(s)
        if g is None:
            return f"sync {s} missing from the trace"
        if g != ref[s]:
            return f"first divergence sync {s}: " + ", ".join(c for c, x, y in zip(COMPARED, g, ref[s]) if x != y)
    if len(got) != len(want):
        return f"trace has {len(got)} sync points, reference {len(want)}"
    return None


class Procs:
    """Child processes of this tool; stop_all() kills the ones still running (fail-fast)."""

    def __init__(self) -> None:
        self.lock = threading.Lock()
        self.live: set[subprocess.Popen] = set()
        self.stopped = threading.Event()

    def run(self, cmd: list[str], env: dict | None = None, cwd: Path | None = None,
            timeout: int = 7200) -> tuple[int, str, str]:
        if self.stopped.is_set():
            return -1, "", "stopped"
        p = subprocess.Popen(cmd, env=env, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
                             errors="replace")
        with self.lock:
            self.live.add(p)
        try:
            out, err = p.communicate(timeout=timeout)
        except subprocess.TimeoutExpired:
            _kill_tree(p.pid)
            out, err = p.communicate()
            err += f"\ntimed out after {timeout} s"
        finally:
            with self.lock:
                self.live.discard(p)
        return (-1 if self.stopped.is_set() and p.returncode else p.returncode), out, err

    def stop_all(self) -> None:
        self.stopped.set()
        with self.lock:
            for p in list(self.live):
                if p.poll() is None:
                    _kill_tree(p.pid)


def _kill_tree(pid: int) -> None:
    """End one child process and its descendants (python -> runner), by PID."""
    if sys.platform == "win32":
        subprocess.run(["taskkill", "/F", "/T", "/PID", str(pid)], capture_output=True)
    else:
        try:
            os.kill(pid, 9)
        except OSError:
            pass


def check_cmd(*args: str) -> list[str]:
    return [sys.executable, str(CHECK), *args]


def last_json(text: str) -> dict | None:
    for line in reversed(text.strip().splitlines()):
        try:
            v = json.loads(line)
            if isinstance(v, dict):
                return v
        except ValueError:
            continue
    return None


def native_supported() -> bool:
    """The native target (phase 5): `xmen2check.py build --native` exists."""
    r = subprocess.run(check_cmd("build", "--help"), capture_output=True, text=True)
    return "--native" in r.stdout


def file_sha1(p: Path) -> str:
    h = hashlib.sha1()
    with open(p, "rb") as f:
        for b in iter(lambda: f.read(1 << 20), b""):
            h.update(b)
    return h.hexdigest()


def cache_root() -> Path:
    """.git/xmen2-cache of the repository (the common dir: one for every worktree)."""
    r = subprocess.run(["git", "-C", str(common.ROOT), "rev-parse", "--git-common-dir"], capture_output=True, text=True)
    g = Path(r.stdout.strip()) if r.returncode == 0 and r.stdout.strip() else common.ROOT / ".git"
    if not g.is_absolute():
        g = (common.ROOT / g).resolve()
    return g / "xmen2-cache"


def _trace_args(refs: Path, run: str, mode: str, out: Path, frames: int | None) -> list[str]:
    args = ["trace", "--mode", mode, "--run", run, "--input", str(refs / f"{run}.input"),
            "--replay", str(refs / f"{run}-recomp-sync"), "--out", str(out), "--json"]
    return args + (["--frames", str(frames)] if frames else [])


def stepped_gen_ref(procs: Procs, refs: Path, run: str, frames: int | None = None,
                    work: Path | None = None, fresh: bool = False) -> dict:
    """The run's reference rows from this build's XMen2SteppedGen: {"ok", "csv", "made", "s",
    "detail"}. A cached full reference also serves a `frames` limit; otherwise one with exactly
    that many sync points is made and cached (`fresh`: made again even if cached)."""
    exe = common.BUILD / "Release" / STEPPED_GEN if sys.platform == "win32" else common.BUILD / STEPPED_GEN
    if not exe.is_file():
        return {"ok": False, "detail": f"no {exe}: build first", "made": False, "s": 0.0}
    inp, rsync = refs / f"{run}.input", refs / f"{run}-recomp-sync" / "sync.csv"
    base = hashlib.sha1(f"{file_sha1(exe)}|{file_sha1(inp)}|{file_sha1(rsync)}".encode()).hexdigest()
    root = cache_root() / "stepped-gen"
    for n in [] if fresh else ([0, frames] if frames else [0]):
        c = root / f"{run}-{base[:16]}-{n or 'all'}" / "sync.csv"
        if c.is_file():
            return {"ok": True, "csv": c, "made": False, "s": 0.0, "detail": f"cached {c.parent.name}"}
    t0 = time.time()
    out = (work or common.BUILD / "refs-work") / f"{run}-stepped-gen-{frames or 'all'}"
    if out.exists():
        shutil.rmtree(out)
    rc, so, se = procs.run(check_cmd(*_trace_args(refs, run, "stepped-gen", out, frames)))
    j = last_json(so)
    s = round(time.time() - t0, 1)
    if rc != 0 or j is None or not (out / "sync.csv").is_file():
        return {"ok": False, "made": True, "s": s,
                "detail": "stopped" if rc == -1 else "stepped-gen: " + (se.strip().splitlines() or ["trace failed"])[-1][:300]}
    if any(j.get(k) for k in COUNTERS):
        return {"ok": False, "made": True, "s": s, "detail": "stepped-gen: " + ", ".join(f"{k} {j.get(k)}" for k in COUNTERS)}
    dst = root / f"{run}-{base[:16]}-{frames or 'all'}"
    dst.mkdir(parents=True, exist_ok=True)
    tmp = dst / f"sync.csv.{os.getpid()}.tmp"
    shutil.copyfile(out / "sync.csv", tmp)
    os.replace(tmp, dst / "sync.csv")   # atomic: another worktree may be making the same one
    (dst / "meta.json").write_text(json.dumps({"run": run, "frames": frames, "key": base, "exe": str(exe),
                                               "fps": j.get("fps"), "sync_points": j.get("sync_points")}), encoding="utf-8")
    shutil.rmtree(out, ignore_errors=True)
    return {"ok": True, "csv": dst / "sync.csv", "made": True, "s": s, "fps": j.get("fps"), "detail": f"made {dst.name}"}


def trace(procs: Procs, refs: Path, run: str, mode: str, out: Path, ref_csv: Path, frames: int | None = None,
          env: dict | None = None) -> dict:
    """One contract trace of `run` in `mode` (stepped, handc, native), compared with the
    reference rows `ref_csv` (their first `frames` sync points). Its output directory is removed
    after a pass."""
    t0 = time.time()
    if out.exists():
        shutil.rmtree(out)
    rc, so, se = procs.run(check_cmd(*_trace_args(refs, run, mode, out, frames)), env=env)
    res = {"run": run, "s": round(time.time() - t0, 1)}
    j = last_json(so)
    if rc != 0 or j is None:
        return {**res, "ok": False, "detail": "stopped" if rc == -1 else (se.strip().splitlines() or ["trace failed"])[-1][:300]}
    bad = compare_sync(ref_csv, out / "sync.csv", frames)
    counts = {k: j.get(k) for k in COUNTERS}
    if not bad and any(counts.values()):
        bad = ", ".join(f"{k} {v}" for k, v in counts.items())
    res.update(ok=bad is None, detail=bad or f"{j.get('sync_points')} sync points equal", fps=j.get("fps"),
               sync_points=j.get("sync_points"), exe_sha1=j.get("exe_sha1"))
    if bad is None:
        shutil.rmtree(out, ignore_errors=True)
    return res
