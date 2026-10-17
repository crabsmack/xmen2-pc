"""Phase 5 (xmen2disasm docs/phases/P5-port.md, P3 "No 68000 left"): what the native runner's
code still needs from the 68000 machine, read from the files the build wrote. Reports only what
the object files and the link map say.

    uv run tools/native_scan.py [--json]       # the scan of an existing build (xmen2check.py build --native
                                               # builds first, then runs this)

objects  every object of the target (XMen2Native.vcxproj), mapped back to its source: the hand-C
         wrapper build/bridge/handc/<ADDR>.cpp is game/c/<ADDR>.c. For game/ sources and the
         bridge: the UNDEFINED external symbols of its COFF symbol table, by category (CATEGORIES
         below). That is everything the object asks the rest of the program for.
totals   over the game/c objects (the hand C and the game/include code inlined into it):
         references per category, the number of files with any, and the distinct symbols.
link     the target's /MAP: the 68000-machine symbols the exe still DEFINES, by category
         (LINK_CATEGORIES), with the object (lib:object) that defines each.
"""
from __future__ import annotations

import argparse
import collections
import csv
import json
import re
import struct
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

# --- COFF ------------------------------------------------------------------------------------

BIGOBJ_CLASSID = bytes.fromhex("C7A1BAD1EEBAA94BAF20FAF66AA4DCB8")  # {D1BAA1C7-BAEE-4ba9-AF20-FAF66AA4DCB8}
IMAGE_SYM_CLASS_EXTERNAL = 2
IMAGE_SYM_CLASS_WEAK_EXTERNAL = 105


def coff_symbols(path: Path) -> list[tuple[str, int, int, int]]:
    """(name, section number, value, storage class) of every symbol record of a COFF object,
    regular or /bigobj; auxiliary records skipped. An LTCG or import object has no COFF symbol
    table: that is an error (the scan would miss its references)."""
    with open(path, "rb") as f:
        head = f.read(56)
        sig1, sig2 = struct.unpack_from("<HH", head, 0)
        if sig1 == 0 and sig2 == 0xFFFF:
            version = struct.unpack_from("<H", head, 4)[0]
            if version >= 2 and head[12:28] == BIGOBJ_CLASSID:
                symtab, nsyms = struct.unpack_from("<II", head, 48)
                rec, secfmt = 20, "<i"
            else:
                raise ValueError(f"{path}: not a plain COFF object (LTCG or import object)")
        else:
            symtab, nsyms = struct.unpack_from("<II", head, 8)
            rec, secfmt = 18, "<h"
        if not nsyms:
            return []
        f.seek(symtab)
        table = f.read(nsyms * rec)
        strsize = struct.unpack("<I", f.read(4))[0]
        strings = f.read(max(0, strsize - 4))
    out = []
    i = 0
    while i < nsyms:
        o = i * rec
        raw = table[o:o + 8]
        if raw[:4] == b"\0\0\0\0":
            off = struct.unpack_from("<I", raw, 4)[0] - 4
            end = strings.find(b"\0", off)
            name = strings[off:end if end >= 0 else None].decode("latin-1")
        else:
            name = raw.rstrip(b"\0").decode("latin-1")
        value = struct.unpack_from("<I", table, o + 8)[0]
        sec = struct.unpack_from(secfmt, table, o + 12)[0]
        sclass = table[o + rec - 2]
        naux = table[o + rec - 1]
        out.append((name, sec, value, sclass))
        i += 1 + naux
    return out


def undefined_externals(path: Path) -> list[str]:
    """Symbols the object references but doesn't define: external, section 0 (undefined), value
    0 (a non-zero value is a common definition), plus weak externals."""
    return sorted({n for n, sec, val, cls in coff_symbols(path)
                   if (cls == IMAGE_SYM_CLASS_EXTERNAL and sec == 0 and val == 0) or cls == IMAGE_SYM_CLASS_WEAK_EXTERNAL})


def common_definitions(path: Path) -> list[str]:
    """C tentative definitions (`uint8_t *g_ram;`): external, section 0, value = the size. The
    linker allocates them itself, so its map names no object for them (`<common>`)."""
    return [n for n, sec, val, cls in coff_symbols(path) if cls == IMAGE_SYM_CLASS_EXTERNAL and sec == 0 and val]


# --- MSVC names -> a readable qualified name --------------------------------------------------

def qualified(sym: str) -> str:
    """'?call_slots@xm@@YAIIPEBIH@Z' -> 'xm::call_slots'; '?g_frame@xm@@3IA' -> 'xm::g_frame';
    extern "C" names stay; '__imp_X' -> 'X'. Operators, templates and other special names
    (starting '??' or '?$') come back as they are: no category matches them."""
    if sym.startswith("__imp_"):
        sym = sym[6:]
    if not sym.startswith("?") or sym.startswith("??") or sym.startswith("?$"):
        return sym
    body = sym[1:]
    end = body.find("@@")
    if end < 0:
        return sym
    parts = body[:end].split("@")
    if not parts or not all(re.fullmatch(r"[A-Za-z_]\w*", p) for p in parts):
        return sym
    return "::".join(reversed(parts[1:])) + ("::" if len(parts) > 1 else "") + parts[0]


# --- categories ------------------------------------------------------------------------------
# What a game object can need from the 68000 machine, by name. Derived from what
# game/runtime/handc_rt.cpp, asm_conv.cpp, the bridge (tools/bridge.py) and the engine
# (segagenesisrecomp runner/include/genesis_runtime.h) export for the C to use.

CPU68 = {
    # 68000 registers and the stack, and calls in its convention (handc_rt.cpp, asm_conv.cpp)
    "g_cpu",
    "xm::call_slots", "xm::call_raw", "xm::jump_raw", "xm::sp_reserve", "xm::sp_restore", "xm::ret_from_c",
    "xm_slot_arg", "xm_set_d0", "xm_reg", "xm_set_reg", "xm_regs_save", "xm_regs_restore", "xm_slot_return",
    "CPU_GetSR", "CPU_SetSR", "CPU_GetSP",
    # the d0 a call leaves when the original returns no value (NO_RETURN_VALUE, slot entries)
    "xm::g_frame", "xm::g_seq", "xm::g_novalue", "xm::g_retnv",
    # the V-int taken where the program waits for it (IRQ_WAIT -> x68k_wait_vint)
    "xm::irq_wait",
    # engine: exact-mode scheduler, interrupts and calls (genesis_runtime.h)
    "g_xclk", "g_xstart", "g_xend", "g_xpc", "g_xrts", "g_xa7min", "g_xinsn", "g_xtrace", "g_xhw_hook",
    "g_xinsn_hook", "g_xirq_taken", "g_xirq_hold", "call_by_address", "m68k_trap_vector", "m68k_illegal_trap",
    "genesis_stop_until_interrupt", "g_rte_pending_ptr", "g_split_sp_popped", "g_early_return",
    "g_cycle_accumulator", "g_vblank_threshold", "g_native_insn_count", "runtime_request_vblank",
}
CPU68_RE = re.compile(r"^(x68k_|recomp_|glue_|hybrid_|m68ki_|xm_slot_|hcslot_|conv_slot_|conv_call_|regs_call_)")
EMU_RAM = {"g_ram", "g_ram_ffff", "genesis_ram_init"}
BUS = {"m68k_read8", "m68k_read16", "m68k_read32", "m68k_write8", "m68k_write16", "m68k_write32",
       "vdp_write_data", "vdp_write_ctrl", "vdp_read_data", "vdp_read_status"}
BUS_RE = re.compile(r"^(dev|io|z80|vdp|psg|ym)_(read|write)\w*$|^genesis_bus_\w+$")
ROM_IMAGE = {"g_rom"}
FNID = {"xm::fn_value", "xm::fn_host", "xm::fn_enter", "xm::fn_leave", "xm::fn_current"}
GENERATED_RE = re.compile(r"^func_(?:body_)?[0-9A-F]{6}$")
CATEGORIES = ("cpu68", "emu_ram", "bus", "rom_image", "fnid", "generated")


def category(q: str, game_names: set[str]) -> str:
    """The category of an undefined symbol (qualified name): one of CATEGORIES, `game` (another
    game function, called through the bridge: no 68000 dependency of its own) or `other`
    (C runtime, xm::fatal...)."""
    if GENERATED_RE.match(q):
        return "generated"
    if q in EMU_RAM:
        return "emu_ram"
    if q in ROM_IMAGE:
        return "rom_image"
    if q in BUS or BUS_RE.match(q):
        return "bus"
    if q in FNID:
        return "fnid"
    if q in CPU68 or CPU68_RE.match(q):
        return "cpu68"
    if q in game_names:
        return "game"
    return "other"


def game_function_names(src: Path) -> set[str]:
    """Every function xmen2_funcs.h declares (and hc_<Name>, its C under its own file's name)."""
    names = set()
    for line in (src / "xmen2_funcs.h").read_text(encoding="utf-8").splitlines():
        m = re.search(r"\b([A-Za-z_]\w*)\s*\([^;]*\)\s*;\s*/\*\s*\$[0-9A-F]{6}", line)
        if m:
            names.add(m.group(1))
            names.add("hc_" + m.group(1))
    return names


# --- the target's objects ----------------------------------------------------------------------

def target_objects(build: Path, target: str, config: str = "Release") -> list[tuple[Path, Path]]:
    """(source, object) of every C/C++ source of the target, from its .vcxproj: the object is
    IntDir/<stem>.obj unless an ObjectFileName says otherwise."""
    proj = (build / f"{target}.vcxproj").read_text(encoding="utf-8", errors="replace")
    intdir = build / f"{target}.dir" / config
    m = re.search(r"<IntDir Condition=\"'\$\(Configuration\)\|\$\(Platform\)'=='" + config + r"\|x64'\">([^<]+)</IntDir>", proj)
    if m:
        intdir = build / m.group(1).replace("\\", "/").rstrip("/")
    out = []
    for m in re.finditer(r"<ClCompile Include=\"([^\"]+)\"\s*(/>|>(.*?)</ClCompile>)", proj, re.S):
        src = Path(m.group(1))
        obj = intdir / (src.stem + ".obj")
        if m.group(3):
            o = re.search(r"<ObjectFileName Condition=\"'\$\(Configuration\)\|\$\(Platform\)'=='" + config +
                          r"\|x64'\">([^<]+)</ObjectFileName>", m.group(3))
            if o and not o.group(1).endswith(("\\", "/", "$(IntDir)")):
                obj = Path(o.group(1).replace("$(IntDir)", str(intdir) + "/"))
                if not obj.is_absolute():
                    obj = build / obj
        out.append((src, obj))
    return out


ROOT_PREFIX = str(common.ROOT.resolve()).replace("\\", "/").rstrip("/").lower() + "/"


def source_key(src: Path) -> str:
    """game/c/<ADDR>.c for a hand-C wrapper (build/bridge/handc/<ADDR>.cpp); else the path
    relative to the repo, or as it is outside it."""
    w = re.fullmatch(r"([0-9A-Fa-f]{6})\.cpp", src.name)
    if w and src.parent.name == "handc":
        return f"game/c/{w.group(1).upper()}.c"
    s = str(src).replace("\\", "/")
    return s[len(ROOT_PREFIX):] if s.lower().startswith(ROOT_PREFIX) else s


# --- the link map --------------------------------------------------------------------------------

LINK_CATEGORIES = ("cpu_core", "interpreter", "recomp_runtime", "generated", "emu_ram")
MAP_LINE = re.compile(r"^\s*([0-9a-fA-F]{4}):([0-9a-fA-F]{8})\s+(\S+)\s+([0-9a-fA-F]{8,16})\s+(.*?)\s*$")
GEN_OBJ = re.compile(r"^xmen2s_part\d+\.obj$")
GEN_RUNTIME_OBJ = re.compile(r"^xmen2s_(dispatch|layout|cycles)\.obj$")
# What the compiler makes for an object's own code and data, not symbols of their own: unwind and
# pdata records ($unwind$f, $pdata$f, $chain$, $LN labels), string literals (??_C@), float
# constants (__real@, __xmm@), RTTI and vtables. They go with their object. Also the C runtime's
# inline functions (printf/scanf family, their option storage): the map names whichever object the
# linker kept the COMDAT from, which says nothing about that object.
COMPILER_LABEL = re.compile(r"^(\$|\?\?_C@|__real@|__xmm@|__ymm@|\?\?_R|\?\?_7)"
                            r"|^_{0,2}v?[sfc]?n?w?(printf|scanf)(_s)?(_l)?$|^__local_stdio_|^\?_OptionsStorage@")
RECOMP_NAMES = {"x68k_entry", "call_by_address", "genesis_log_dispatch_miss", "genesis_write_dispatch_miss_evidence",
                "g_miss_count_any", "g_miss_last_addr", "g_miss_last_frame", "g_miss_unique_addrs", "g_miss_unique_count"}
CPU_CORE_NAMES = {"g_cpu", "m68k_trap_vector", "m68k_illegal_trap", "genesis_stop_until_interrupt"}


def link_category(q: str, obj: str) -> str | None:
    """Which 68000-machine category a symbol the exe defines belongs to (None: not one), by the
    object that defines it and its name:
      generated       func_ definitions of the generated code (xmen2s_part*.obj)
      interpreter     the Tier-3 68000 interpreter (m68k_interp.obj), its decoder, m68ki_/hybrid_ state
      emu_ram         the emulated work RAM (ram_views.obj: g_ram, g_ram_ffff)
      cpu_core        the 68000 state and core: g_cpu, the bus front (m68k_read*/write*), the exact
                      scheduler (exact_sched.obj: x68k_*, interrupts), traps, CPU_GetSR/SetSR/GetSP
      recomp_runtime  calls and dispatch in the 68000 convention: glue.obj (recomp_*, dispatch,
                      the game fiber, the generated code's bus glue), the dispatch table and the
                      rest of the generated objects, XENTRY (x68k_entry), the C<->68000 call layer
                      (handc_rt.obj, asm_conv.obj) and the bridge's slot entries (func_/hcslot_/
                      conv_ entries in xmen2_bridge.obj; its C-callable wrappers and the function
                      table are not counted)"""
    o = obj.split(":")[-1].lower()
    if GEN_OBJ.match(o):
        return "generated" if GENERATED_RE.match(q) else "recomp_runtime"
    if GEN_RUNTIME_OBJ.match(o):
        return "recomp_runtime"
    if o in ("m68k_interp.obj", "m68k_decoder.obj") or "m68ki" in q or q.startswith(("hybrid_", "g_hybrid_")):
        return "interpreter"
    if o == "ram_views.obj" or q in EMU_RAM:
        return "emu_ram"
    if q in CPU_CORE_NAMES or q in BUS or q.startswith("CPU_"):
        return "cpu_core"
    if q in RECOMP_NAMES or q.startswith(("recomp_", "glue_")):
        return "recomp_runtime"
    if o in ("handc_rt.obj", "asm_conv.obj", "glue.obj"):
        return "recomp_runtime"
    if o == "xmen2_bridge.obj":
        return "recomp_runtime" if re.match(r"^(func_|hcslot_|conv_slot_|conv_call_|regs_call_)", q) else None
    if o == "exact_sched.obj" or q.startswith(("x68k_", "g_x")):
        return "cpu_core"
    return None


def read_map(path: Path) -> list[tuple[str, str]]:
    """(symbol, lib:object) of every public and static symbol a /MAP lists."""
    out = []
    section = None
    for line in path.read_text(encoding="latin-1").splitlines():
        if "Publics by Value" in line:
            section = "public"
            continue
        if line.strip().startswith("Static symbols"):
            section = "static"
            continue
        if line.strip().startswith("entry point at") or line.strip().startswith("Exports"):
            continue
        if not section:
            continue
        m = MAP_LINE.match(line)
        if not m:
            continue
        rest = m.group(5).split()
        if not rest:
            continue
        out.append((m.group(3), rest[-1]))
    return out


# --- the scan ------------------------------------------------------------------------------------

def scan(build: Path, target: str, exe: Path) -> dict:
    t0 = time.time()
    src_dir = build / "handc_src" if (build / "handc_src" / "xmen2_funcs.h").exists() else common.GAME / "include"
    game_names = game_function_names(src_dir)
    objects: dict[str, dict] = {}
    missing = []
    commons: dict[str, list[str]] = {}   # tentative definition -> the objects declaring it
    for src, obj in target_objects(build, target):
        key = source_key(src)
        if not obj.is_file():
            missing.append(str(obj))
            continue
        if not (key.startswith("game/") or key.endswith("bridge/xmen2_bridge.cpp")):
            # the engine and the generated code are the machine itself: only their C tentative
            # definitions are read (the map lists those as <common>)
            if obj.suffix == ".obj" and src.suffix.lower() == ".c":
                for n in common_definitions(obj):
                    commons.setdefault(n, []).append(obj.name)
            continue
        if src.suffix.lower() == ".c":
            for n in common_definitions(obj):
                commons.setdefault(n, []).append(obj.name)
        by = {c: [] for c in CATEGORIES}
        n_game = n_other = 0
        for sym in undefined_externals(obj):
            q = qualified(sym)
            c = category(q, game_names)
            if c == "game":
                n_game += 1
            elif c == "other":
                n_other += 1
            else:
                by[c].append(q)
        rec = {c: len(by[c]) for c in CATEGORIES}
        rec.update(game=n_game, other=n_other, symbols={c: sorted(v) for c, v in by.items() if v})
        objects[key] = rec
    hand = {k: v for k, v in objects.items() if k.startswith("game/c/")}
    totals = {c: sum(v[c] for v in hand.values()) for c in CATEGORIES}
    totals["files"] = {c: sum(1 for v in hand.values() if v[c]) for c in CATEGORIES}
    totals["objects"] = len(hand)
    # each symbol with the number of game/c files that reference it
    totals["symbols"] = {c: dict(sorted(collections.Counter(
        s for v in hand.values() for s in v["symbols"].get(c, [])).items())) for c in CATEGORIES}
    runtime = {k: v for k, v in objects.items() if not k.startswith("game/c/")}
    runtime_totals = {c: sum(v[c] for v in runtime.values()) for c in CATEGORIES}
    # the link map
    mp = exe.with_suffix(".map")
    link: dict = {"map": str(mp)}
    if mp.is_file():
        with open(common.GAME / "xmen2.functions.csv", newline="") as fh:
            game_addrs = {r["addr"].upper() for r in csv.DictReader(fh)}
        forbidden, cats = [], dict.fromkeys(LINK_CATEGORIES, 0)
        seen = set()
        for sym, obj in read_map(mp):
            if COMPILER_LABEL.match(sym):
                continue
            q = qualified(sym)
            if obj == "<common>" and sym in commons:
                obj = ",".join(sorted(set(commons[sym])))   # the objects whose C declares it
            c = link_category(q, obj.split(",")[0])
            if c is None or (q, obj) in seen:
                continue
            seen.add((q, obj))
            cats[c] += 1
            e = {"symbol": q, "category": c, "object": obj}
            if q != sym:
                e["decorated"] = sym
            forbidden.append(e)
        link.update(categories=cats, forbidden=forbidden,
                    generated_game_functions=sum(1 for e in forbidden if e["category"] == "generated"
                                                 and e["symbol"][-6:] in game_addrs),
                    objects={c: sorted({e["object"] for e in forbidden if e["category"] == c}) for c in LINK_CATEGORIES})
    else:
        link["error"] = "no link map"
    return {"map": str(mp), "objects": objects, "totals": totals, "runtime_totals": runtime_totals,
            "link": link, "missing_objects": missing, "scan_seconds": round(time.time() - t0, 2),
            "ok": not missing and mp.is_file()}


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--target", default="XMen2Native")
    a = ap.parse_args()
    exe = common.BUILD / "Release" / (a.target + (".exe" if sys.platform == "win32" else ""))
    res = scan(common.BUILD, a.target, exe)
    if not a.json:
        res = {k: v for k, v in res.items() if k != "objects"}
        res["link"] = {k: v for k, v in res["link"].items() if k != "forbidden"}
    print(json.dumps(res, indent=None if a.json else 1))
    return 0 if res["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
