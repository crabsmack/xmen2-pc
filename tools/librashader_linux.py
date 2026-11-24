"""librashader.so for shader_preset on Linux (NOTES "Shader presets"): librashader publishes Windows
libraries only, so its C API is built here from the source of the release the Windows zip ships
(v0.12.0), pinned by the tag's commit, with the lock file it carries (`--locked`). Run on Linux (P9's
build runs it in WSL through `xmen2check.py native-build`); needs git, cargo (rustup), a C++ compiler.

The result is cached per version, outside every build dir (a build takes minutes):
    $XMEN2_LIBRASHADER_CACHE or ~/.cache/xmen2/librashader-<version>/librashader.so
native.cmake copies it beside the program. Prints the path (a JSON line with --json); exit 1 when it
can't be built (no cargo, offline), and the game then says the library is missing.

usage: python3 tools/librashader_linux.py [--json] [--force]
"""
from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path

VERSION = "0.12.0"
REPO = "https://github.com/SnowflakePowered/librashader.git"
TAG = f"librashader-v{VERSION}"
COMMIT = "87e8a97b50516d997defeaa168173dcd185d4022"   # the tag's commit (an annotated tag: 5537e838)


def cache_dir() -> Path:
    env = os.environ.get("XMEN2_LIBRASHADER_CACHE")
    return Path(env) if env else Path.home() / ".cache" / "xmen2" / f"librashader-{VERSION}"


def find_cargo() -> str | None:
    c = shutil.which("cargo")
    if c:
        return c
    c = Path.home() / ".cargo" / "bin" / "cargo"
    return str(c) if c.is_file() else None


def run(args: list[str], cwd: Path | None = None, log=None) -> int:
    return subprocess.run(args, cwd=cwd, stdout=log, stderr=subprocess.STDOUT if log else None).returncode


def build(force: bool) -> tuple[Path | None, str]:
    out = cache_dir()
    so = out / "librashader.so"
    if so.is_file() and not force:
        return so, "cached"
    cargo = find_cargo()
    if not cargo:
        return None, "no cargo (install Rust: https://rustup.rs)"
    src = out / "src"
    out.mkdir(parents=True, exist_ok=True)
    log_path = out / "build.log"
    with open(log_path, "w", encoding="utf-8") as log:
        if not (src / ".git").is_dir():
            if src.exists():
                shutil.rmtree(src)
            if run(["git", "clone", "--depth", "1", "--branch", TAG, REPO, str(src)], log=log):
                return None, f"clone failed, see {log_path}"
        head = subprocess.run(["git", "rev-parse", "HEAD"], cwd=src, capture_output=True, text=True).stdout.strip()
        if head != COMMIT:
            return None, f"{TAG} is {head}, not the pinned {COMMIT}"
        env = dict(os.environ, PATH=str(Path(cargo).parent) + os.pathsep + os.environ.get("PATH", ""))
        # the C API as upstream builds its releases (librashader-build-script: cargo build of librashader-capi
        # with the "optimized" profile and its default features, then liblibrashader_capi.so renamed)
        r = subprocess.run([cargo, "run", "--locked", "-p", "librashader-build-script", "--", "--profile", "optimized",
                            "--", "--locked"], cwd=src, env=env, stdout=log, stderr=subprocess.STDOUT)
        if r.returncode:
            return None, f"cargo build failed, see {log_path}"
    built = next((p for p in (src / "target" / "optimized" / "librashader.so",) if p.is_file()), None)
    if not built:
        return None, f"no librashader.so under {src / 'target' / 'optimized'}, see {log_path}"
    tmp = out / "librashader.so.tmp"
    shutil.copy2(built, tmp)
    tmp.replace(so)
    return so, "built"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--force", action="store_true", help="build again even when cached")
    a = ap.parse_args()
    if not sys.platform.startswith("linux"):
        sys.exit("librashader_linux.py runs on Linux (the Windows build fetches the upstream DLL)")
    so, how = build(a.force)
    if a.json:
        print(json.dumps({"ok": so is not None, "so": str(so) if so else None, "how": how, "version": VERSION,
                          "commit": COMMIT}))
    else:
        print(so if so else f"librashader.so not built: {how}", file=sys.stdout if so else sys.stderr)
    return 0 if so else 1


if __name__ == "__main__":
    sys.exit(main())
