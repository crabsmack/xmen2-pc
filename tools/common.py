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


def tool(name: str) -> str:
    """cmake / ninja from the uv environment (dev dependencies), else PATH."""
    exe = Path(sys.executable).parent / (name + (".exe" if sys.platform == "win32" else ""))
    if exe.exists():
        return str(exe)
    found = shutil.which(name)
    if not found:
        sys.exit(f"{name} not found (uv sync installs it)")
    return found
