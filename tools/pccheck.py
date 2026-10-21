"""Compile hand-written C files in PC mode (C++17, MSVC) on their own: a quick check that a
game/c file builds as the runners build it, without touching the build tree (phase 4).

    uv run tools/pccheck.py game/c/1F9C92.c [more.c ...] [--files-from LIST]
    uv run tools/pccheck.py --all [--jobs N]

Each file is compiled like handc.cmake does (-D<Name>=hc_<Name>, XMEN2_FN, C++17, the game
headers), objects go to a temporary directory. The compiles run on a pool of --jobs (default:
every hardware thread), one cl per file; failures are printed first. Exit 0 when every file
compiles cleanly. Safe to run beside other work: no runner, no shared build directory.
MSVC only (Windows), like the phase-5 harness until its Linux step.
"""
from __future__ import annotations

import argparse
import concurrent.futures
import csv
import hashlib
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402


def vcvars() -> Path:
    vswhere = Path(os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)")) / "Microsoft Visual Studio" / "Installer" / "vswhere.exe"
    inst = subprocess.run([str(vswhere), "-latest", "-property", "installationPath"], capture_output=True, text=True).stdout.strip()
    p = Path(inst) / "VC" / "Auxiliary" / "Build" / "vcvars64.bat"
    if not p.exists():
        sys.exit("vcvars64.bat not found (Visual Studio 2022 with C++)")
    return p


def msvc_env() -> dict[str, str]:
    """The environment vcvars64.bat sets up (cl on PATH, INCLUDE, LIB), cached per vcvars file
    and base PATH in the temp directory: running the .bat costs seconds, a compile far less."""
    bat = vcvars()
    key = hashlib.sha1(f"{bat}|{bat.stat().st_mtime_ns}|{os.environ.get('PATH', '')}".encode()).hexdigest()[:16]
    cache = Path(tempfile.gettempdir()) / f"xmen2pc_vcvars_{key}.json"
    if cache.exists():
        try:
            return json.loads(cache.read_text(encoding="utf-8"))
        except ValueError:
            pass
    r = subprocess.run(f'cmd /c ""{bat}" >nul && set"', capture_output=True, text=True, shell=True)
    env = dict(l.split("=", 1) for l in r.stdout.splitlines() if "=" in l and not l.startswith("="))
    if r.returncode or not any(k.upper() == "INCLUDE" for k in env):
        sys.exit(f"vcvars64.bat failed ({r.returncode})")
    cache.write_text(json.dumps(env), encoding="utf-8")
    return env


def find_cl(env: dict[str, str]) -> str:
    path = next((v for k, v in env.items() if k.upper() == "PATH"), "")
    return next((str(Path(d) / "cl.exe") for d in path.split(";") if d and (Path(d) / "cl.exe").is_file()), "cl")


def names() -> dict[str, str]:
    with open(common.GAME / "xmen2.functions.csv", newline="") as f:
        return {r["addr"].upper(): r["name"] for r in csv.DictReader(f)}


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("files", nargs="*", type=Path)
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--files-from", type=Path, help="a file listing the files, one per line")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 8, help="compiles at once (default: every thread)")
    a = ap.parse_args()
    if a.files_from:
        a.files += [Path(l.strip()) for l in a.files_from.read_text(encoding="utf-8").splitlines() if l.strip()]
    files = sorted((common.GAME / "c").glob("*.c")) if a.all else [f.resolve() for f in a.files]
    if not files:
        ap.print_help()
        return 1
    fn = names()
    inc = [common.GAME / "include", common.ROOT / "platform", common.ENGINE / "runner" / "include", common.ENGINE / "runner"]
    bad = 0
    with tempfile.TemporaryDirectory(prefix="xmen2pc_") as td:
        cmds = []
        for f in files:
            addr = f.stem.upper()
            name = fn.get(addr)
            if not name or not re.fullmatch(r"[0-9A-F]{6}", addr):
                print(f"{f.name}: not named after a function start")
                bad += 1
                continue
            args = ["/nologo", "/c", "/TP", "/std:c++17", "/EHsc", "/W3", "/utf-8", "/wd4996",
                    f"/D{name}=hc_{name}", f"/DXMEN2_FN=0x{addr}", *[f"/I{p}" for p in inc],
                    f"/Fo{Path(td) / (addr + '.obj')}", str(f)]
            cmds.append((f, args))
        env = msvc_env() if cmds else {}
        cl = find_cl(env)

        def one(item):
            f, args = item
            r = subprocess.run([cl, *args], env=env, capture_output=True, text=True, errors="replace")
            msgs = [l for l in (r.stdout + r.stderr).splitlines() if re.search(r": (error|warning) ", l)]
            ok = r.returncode == 0 and not any(": error " in m for m in msgs)
            return f, ok, msgs

        with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, a.jobs)) as ex:
            results = list(ex.map(one, cmds))
        # failures first, then the rest in file order (each with the line a serial run printed)
        for f, ok, msgs in sorted(results, key=lambda r: r[1]):
            bad += not ok
            print(f"{f.name}: {'OK' if ok and not msgs else ('OK (warnings)' if ok else 'FAILED')}")
            for m in msgs[:15]:
                print("   ", m.strip())
    print(f"{len(files) - bad}/{len(files)} compile as C++ (PC mode)")
    return 1 if bad else 0


if __name__ == "__main__":
    raise SystemExit(main())
