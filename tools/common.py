"""Shared paths and helpers for the XMen2Recomp tools."""
from __future__ import annotations

import hashlib
import os
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GAME = ROOT / "game"
ENGINE = ROOT / "segagenesisrecomp"
DISASM = ROOT / "xmen2disasm"
# The working disassembly checkout next to this repo: reference runs (emu/runs), the corpus
# and the phase checker live there ($XMEN2_DISASM overrides).
DISASM_MAIN = Path(os.environ.get("XMEN2_DISASM") or ROOT.parent / "xmen2disasm")
BUILD = Path(os.environ.get("XMEN2_BUILD") or ROOT / "build")   # dev: a second build tree
ROM = GAME / "xmen2.bin"
ROM_SHA1 = "61409e6cf6065ab67d8952b891d8edcf47777193"
EXE_NAME = "XMen2Recomp.exe" if sys.platform == "win32" else "XMen2Recomp"


def sha1(p: Path) -> str:
    return hashlib.sha1(p.read_bytes()).hexdigest()


def ensure_rom() -> Path:
    """game/xmen2.bin (gitignored): copied from $XMEN2_ROM or a sibling xmen2disasm checkout."""
    if ROM.exists() and sha1(ROM) == ROM_SHA1:
        return ROM
    cands = [Path(os.environ["XMEN2_ROM"])] if os.environ.get("XMEN2_ROM") else []
    cands += [ROOT.parent / "xmen2disasm" / "xmen2.bin", DISASM / "xmen2.bin"]
    for c in cands:
        if c.exists() and sha1(c) == ROM_SHA1:
            shutil.copyfile(c, ROM)
            return ROM
    sys.exit(f"no ROM with SHA-1 {ROM_SHA1}: set XMEN2_ROM to your dump")


def free_memory_gb() -> float | None:
    """Available physical memory (Windows: GlobalMemoryStatusEx; Linux: MemAvailable), or None."""
    try:
        if sys.platform == "win32":
            import ctypes

            class MS(ctypes.Structure):
                _fields_ = [("dwLength", ctypes.c_ulong), ("dwMemoryLoad", ctypes.c_ulong),
                            ("ullTotalPhys", ctypes.c_ulonglong), ("ullAvailPhys", ctypes.c_ulonglong),
                            ("ullTotalPageFile", ctypes.c_ulonglong), ("ullAvailPageFile", ctypes.c_ulonglong),
                            ("ullTotalVirtual", ctypes.c_ulonglong), ("ullAvailVirtual", ctypes.c_ulonglong),
                            ("ullAvailExtendedVirtual", ctypes.c_ulonglong)]
            ms = MS()
            ms.dwLength = ctypes.sizeof(MS)
            return ms.ullAvailPhys / 2**30 if ctypes.windll.kernel32.GlobalMemoryStatusEx(ctypes.byref(ms)) else None
        with open("/proc/meminfo") as f:
            return next((int(line.split()[1]) / 2**20 for line in f if line.startswith("MemAvailable:")), None)
    except (OSError, AttributeError, ValueError):
        return None


class MemGate:
    """`with gate:` around one job of a pool: it starts only while `floor` GB of RAM is free
    ($XMEN2_MIN_FREE_GB, else $PHASE_MIN_FREE_GB, else 3), or when none of this gate's jobs runs, or
    after `max_wait` seconds (2026-10-24: an engine build that leaked ~1 GB per verify process ran
    the machine out of memory at 20 processes). The first wait of a stretch is printed to stderr."""

    def __init__(self, floor: float | None = None, max_wait: float = 120.0):
        import threading
        env = os.environ.get("XMEN2_MIN_FREE_GB") or os.environ.get("PHASE_MIN_FREE_GB")
        self.floor = floor if floor is not None else float(env or 3)
        self.max_wait = max_wait
        self.cv = threading.Condition()
        self.running = self.waiting = 0

    def __enter__(self) -> "MemGate":
        import time
        with self.cv:
            waited, t0 = False, time.time()
            while (self.running > 0 and time.time() - t0 < self.max_wait
                   and (free_memory_gb() or self.floor) < self.floor):
                if not waited:
                    if self.waiting == 0:
                        print(f"  (memory: {free_memory_gb() or 0:.1f} GB free < {self.floor:g}; holding new jobs, "
                              f"{self.running} running)", file=sys.stderr, flush=True)
                    self.waiting += 1
                    waited = True
                self.cv.wait(timeout=2.0)
            if waited:
                self.waiting -= 1
            self.running += 1
        return self

    def __exit__(self, *exc) -> None:
        with self.cv:
            self.running -= 1
            self.cv.notify_all()


def tool(name: str) -> str:
    """cmake / ninja from the uv environment (dev dependencies), else PATH."""
    exe = Path(sys.executable).parent / (name + (".exe" if sys.platform == "win32" else ""))
    if exe.exists():
        return str(exe)
    found = shutil.which(name)
    if not found:
        sys.exit(f"{name} not found (uv sync installs it)")
    return found
