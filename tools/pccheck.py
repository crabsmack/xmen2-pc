"""Compile hand-written C files in PC mode (C++17, MSVC) on their own: a quick check that a
game/c file builds as the runners build it, without touching the build tree (phase 4).

    uv run tools/pccheck.py game/c/1F9C92.c [more.c ...]
    uv run tools/pccheck.py --all

Each file is compiled like handc.cmake does (-D<Name>=hc_<Name>, XMEN2_FN, C++17, the game
headers), objects go to a temporary directory. Exit 0 when every file compiles cleanly.
Safe to run beside other work: no runner, no shared build directory.
"""
from __future__ import annotations

import argparse
import csv
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


def names() -> dict[str, str]:
    with open(common.GAME / "xmen2.functions.csv", newline="") as f:
        return {r["addr"].upper(): r["name"] for r in csv.DictReader(f)}


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("files", nargs="*", type=Path)
    ap.add_argument("--all", action="store_true")
    a = ap.parse_args()
    files = sorted((common.GAME / "c").glob("*.c")) if a.all else [f.resolve() for f in a.files]
    if not files:
        ap.print_help()
        return 1
    fn = names()
    inc = [common.GAME / "include", common.ENGINE / "runner" / "include", common.ENGINE / "runner"]
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
            args = ["cl", "/nologo", "/c", "/TP", "/std:c++17", "/EHsc", "/W3", "/utf-8", "/wd4996",
                    f"/D{name}=hc_{name}", f"/DXMEN2_FN=0x{addr}", *[f"/I{p}" for p in inc],
                    f"/Fo{Path(td) / (addr + '.obj')}", str(f)]
            cmds.append((f, args))
        bat = Path(td) / "run.bat"
        lines = ["@echo off", f'call "{vcvars()}" >nul']
        for f, args in cmds:
            lines.append(" ".join(f'"{x}"' if " " in x else x for x in args) + f" > \"{Path(td) / (f.stem + '.log')}\" 2>&1")
            lines.append(f"echo {f.stem} %ERRORLEVEL%")
        bat.write_text("\r\n".join(lines) + "\r\n")
        r = subprocess.run(["cmd", "/c", str(bat)], capture_output=True, text=True)
        status = dict(l.split() for l in r.stdout.splitlines() if re.fullmatch(r"[0-9A-Fa-f]{6} -?\d+", l.strip()))
        for f, _args in cmds:
            log = (Path(td) / (f.stem + ".log")).read_text(errors="replace")
            msgs = [l for l in log.splitlines() if re.search(r": (error|warning) ", l)]
            ok = status.get(f.stem) == "0" and not any(": error " in m for m in msgs)
            bad += not ok
            print(f"{f.name}: {'OK' if ok and not msgs else ('OK (warnings)' if ok else 'FAILED')}")
            for m in msgs[:15]:
                print("   ", m.strip())
    print(f"{len(cmds) - bad}/{len(cmds)} compile as C++ (PC mode)")
    return 1 if bad else 0


if __name__ == "__main__":
    raise SystemExit(main())
