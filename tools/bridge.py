"""Generate the bridge between hand-written C and generated code (phase 4; P4-decomp.md
"Where things live"). Build output, never committed: tools/xmen2check.py build runs it.

    uv run tools/bridge.py --out build/bridge [--mode stepped|handc|verify]

From game/include/xmen2_funcs.h (a prototype per function) and the game/c/<ADDR>.c files:

  xmen2_bridge.cpp
    - for a function written in C: the C is compiled as hc_<Name> (its handc/<ADDR>.cpp renames
      it), and <Name> is a wrapper every C caller reaches: it counts the entry (logical
      positions, x68k_entry) and calls hc_<Name>. Its slot entry hcslot_<ADDR> takes a call
      from generated code: arguments from the 4-byte stack slots, result to d0, then RTS. In
      the stepped/handc builds that entry IS func_<ADDR>: the recompiler leaves the function
      out (handc_skip.txt), so every generated call and dispatch lands in the C.
    - for a function still generated: <Name> is a stub that pushes the arguments in slots and
      calls the generated code, so C never knows which side a callee is on.
    - the function-address table (xm::fn_value / fn_host): the 68000 value a function's
      address had where the original took it (game/xmen2.fnrefs.csv).
  handc/<ADDR>.cpp  one per C file: `#define Name hc_Name`, `#define XMEN2_FN 0xADDR`, the file
  handc.cmake       XMEN2_HANDC_SOURCES: those wrappers (same compile options, so /MP batches them)
  handc_skip.txt    the addresses whose generated code the stepped/handc builds leave out
"""
from __future__ import annotations

import argparse
import bisect
import csv
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

PROTO = re.compile(r"^(?P<ret>[^()]*?)\s*\b(?P<name>[A-Za-z_]\w*)\s*\((?P<params>[^;]*)\)\s*;\s*/\*\s*\$(?P<addr>[0-9A-F]{6})(?P<tag>[^*]*)\*/")

# Where the hand-written C and its headers are read from: game/ (c/, include/), or a snapshot
# of both in one directory (--src; xmen2check.py build takes one, so that a build compiles one
# consistent state while the C is being edited).
SRC: Path | None = None


def write_if_changed(path: Path, text: str) -> None:
    """Write text (UTF-8, LF) unless the file already holds it: an unchanged output keeps its
    timestamp, so the build doesn't recompile or reconfigure for it."""
    data = text.encode("utf-8")
    if path.exists() and path.read_bytes() == data:
        return
    path.write_bytes(data)


def headers_dir() -> Path:
    return SRC if SRC else common.GAME / "include"


def split_params(s: str) -> list[str]:
    """Split a parameter list at top-level commas (a function-pointer parameter has its own)."""
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
            continue
        depth += (ch == "(") - (ch == ")")
        cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def decl(t: str, name: str) -> str:
    """A declaration of `name` with type t (a function pointer type takes the name inside)."""
    return t.replace("(*)", f"(*{name})", 1) if "(*)" in t else f"{t} {name}"


def parse_protos() -> list[dict]:
    out = []
    for line in (headers_dir() / "xmen2_funcs.h").read_text(encoding="utf-8").splitlines():
        m = PROTO.match(line.strip())
        if not m:
            continue
        ret = m.group("ret").strip()
        params = m.group("params").strip()
        plist = [] if params in ("", "void") else split_params(params)
        types = []
        for i, p in enumerate(plist):
            p = re.sub(r"\s*=[^=].*$", "", p)   # a C++ default argument (PC prototype only)
            if re.search(r"\(\s*\*\s*[A-Za-z_]\w*\s*\)\s*\(", p):
                # "void (*load)(void)": a function pointer; drop its name
                types.append(re.sub(r"\(\s*\*\s*[A-Za-z_]\w*\s*\)", "(*)", p))
                continue
            # "AiCursor *cursor" / "void *" / "short": drop a trailing parameter name
            t = re.sub(r"\b[A-Za-z_]\w*$", "", p).strip() if re.search(r"[\s*][A-Za-z_]\w*$", p) and \
                not re.fullmatch(r"(unsigned\s+|signed\s+)?(char|short|int|long)", p) else p
            types.append(t or p)
        tag = m.group("tag").strip()
        f = {"name": m.group("name"), "ret": ret, "types": types, "addr": int(m.group("addr"), 16),
             "fragment": "fragment" in tag, "asm": "asm" in tag, "regs": None, "ret_reg": None,
             "conv": bool(re.search(r"\bconv\b", tag)), "clobbers": [],
             "noreturn": bool(re.search(r"\bnoreturn\b", tag)),
             "vector": bool(re.search(r"\bvector\b", tag))}
        rm = re.search(r"\bregs\(((?:[^()]|\([^()]*\))*)\)(?:\s*->\s*([ad][0-7]))?", tag)
        if rm:
            # a register convention (hand-written 68000): parameter i comes in register regs[i]
            # ("&d1": a pointer to a value that is in d1 before the call and goes back to d1 after;
            # "a0=$A00036": an input that is always that value, which the C doesn't take)
            # "@8(a6)": the long at a6+8, an argument of the caller's frame (hand-written code that
            # runs inside its caller's frame)
            ent = [r.strip() for r in rm.group(1).split(",") if r.strip()]
            regs = [r for r in ent if "=" not in r]
            consts = [r.split("=") for r in ent if "=" in r]
            if len(regs) != len(types) or not all(re.fullmatch(r"&?[ad][0-7]|@\$?[0-9A-Fa-f]+\(a6\)", r) for r in regs) \
                    or not all(re.fullmatch(r"[ad][0-7]", r) and re.fullmatch(r"\$[0-9A-Fa-f]+", v) for r, v in consts):
                sys.exit(f"xmen2_funcs.h ${f['addr']:06X}: regs({rm.group(1)}) doesn't fit {len(types)} parameters")
            if (ret == "void") != (rm.group(2) is None):
                sys.exit(f"xmen2_funcs.h ${f['addr']:06X}: a register result needs ->reg (and only then)")
            f["regs"], f["ret_reg"] = regs, rm.group(2)
            f["reg_consts"] = [(regno(r), int(v[1:], 16)) for r, v in consts]
        cm = re.search(r"\bclobbers\(([^)]*)\)", tag)
        if cm:
            # registers the hand-written 68000 leaves changed with no meaning (verify skips them)
            f["clobbers"] = [r.strip() for r in cm.group(1).split(",") if r.strip()]
            if not all(re.fullmatch(r"d[0-7]|a[0-6]", r) for r in f["clobbers"]):
                sys.exit(f"xmen2_funcs.h ${f['addr']:06X}: clobbers({cm.group(1)})")
        out.append(f)
    return out


def regno(r: str) -> int:
    """d0-d7 = 0-7, a0-a7 = 8-15."""
    r = r.lstrip("&")
    return int(r[1]) + (8 if r[0] == "a" else 0)


def frame_off(r: str) -> int | None:
    """The offset of a frame operand "@8(a6)" ("@$C(a6)" hex), else None."""
    m = re.fullmatch(r"@(\$)?([0-9A-Fa-f]+)\(a6\)", r)
    return None if not m else int(m.group(2), 16 if m.group(1) else 10)


def pointee(t: str) -> str:
    return re.sub(r"\*\s*$", "", t.strip()).strip()


def regs_call(f: dict) -> list[str]:
    """C -> generated for a register convention: registers saved, inputs set, the code called,
    outputs read back, registers restored (C never sees them)."""
    a = f["addr"]
    L = [f"static {sig(f, f'regs_call_{a:06X}')}", "{", "    uint32_t saved[15]; xm_regs_save(saved);"]
    frame = [(i, frame_off(r)) for i, r in enumerate(f["regs"]) if frame_off(r) is not None]
    if frame:
        # a frame for the code to read its caller's arguments from: a6 points at it
        size = max(o for _i, o in frame) + 4
        L.append(f"    uint32_t sp0 = xm::sp_reserve(0), fr = xm::sp_reserve({size}u);")
        for i, o in frame:
            L.append(f"    m68k_write32(fr + {o}u, xm::slot(a{i}));")
        L.append("    xm_set_reg(14, fr, 4);")
    for r, v in f.get("reg_consts", []):
        L.append(f"    xm_set_reg({r}, 0x{v:08X}u, 4);")
    # value parameters first; a pointer parameter's register only when no value parameter
    # already fills it (regs(d0,d1,&d1): d1 holds the divisor in, the quotient out)
    given = {regno(r) for r in f["regs"] if not r.startswith("&") and frame_off(r) is None}
    for i, (t, r) in enumerate(zip(f["types"], f["regs"])):
        if not r.startswith("&") and frame_off(r) is None:
            L.append(f"    xm_set_reg({regno(r)}, xm::slot(a{i}), 4);")
    for i, (t, r) in enumerate(zip(f["types"], f["regs"])):
        if r.startswith("&") and regno(r) not in given:
            L.append(f"    xm_set_reg({regno(r)}, xm::slot(*a{i}), 4);")
    L.append(f"    xm::call_slots(0x{a:06X}u, nullptr, 0);")
    for i, (t, r) in enumerate(zip(f["types"], f["regs"])):
        if r.startswith("&"):
            L.append(f"    *a{i} = from_slot<{pointee(t)}>(xm_reg({regno(r)}));")
    if f["ret_reg"]:
        L.append(f"    {f['ret']} r = from_slot<{f['ret']}>(xm_reg({regno(f['ret_reg'])}));")
    L.append("    xm_regs_restore(saved);")
    if frame:
        L.append("    xm::sp_restore(sp0);")
    if f["ret_reg"]:
        L.append("    return r;")
    L.append("}")
    return L


def regs_slot(f: dict, target: str, verify: bool) -> list[str]:
    """generated -> C for a register convention: inputs from the registers, outputs to them."""
    L = []
    args = []
    for i, (t, r) in enumerate(zip(f["types"], f["regs"])):
        if r.startswith("&"):
            L.append(f"    {pointee(t)} v{i} = from_slot<{pointee(t)}>(xm_reg({regno(r)}));")
            args.append(f"&v{i}")
        elif frame_off(r) is not None:
            args.append(f"from_slot<{t}>(m68k_read32(xm_reg(14) + {frame_off(r)}u))")
        else:
            args.append(f"from_slot<{t}>(xm_reg({regno(r)}))")
    callx = f"{target}({', '.join(args)})"
    if f["ret_reg"]:
        L.append(f"    auto r = call_c([&] {{ return {callx}; }});" if verify else f"    auto r = {callx};")
    else:
        L.append(f"    {callx};")
        L.append("    xm::g_retnv = false;")
    for i, (t, r) in enumerate(zip(f["types"], f["regs"])):
        if r.startswith("&"):
            L.append(f"    xm_set_reg({regno(r)}, to_u32(v{i}), val_bytes<{pointee(t)}>());")
    if f["ret_reg"]:
        L.append(f"    if (!xm::g_retnv) xm_set_reg({regno(f['ret_reg'])}, to_u32(r), val_bytes<{f['ret']}>());")
    return L


def outs(f: dict) -> list[tuple[int, str]]:
    """The registers a register-convention function hands back, with their C type."""
    o = [(regno(r), pointee(t)) for t, r in zip(f["types"], f["regs"] or []) if r.startswith("&")]
    if f["ret_reg"]:
        o.append((regno(f["ret_reg"]), f["ret"]))
    return o


def hand_c() -> dict[int, Path]:
    d = SRC if SRC else common.GAME / "c"
    return {int(p.stem, 16): p for p in sorted(d.glob("*.c")) if re.fullmatch(r"[0-9A-Fa-f]{6}", p.stem)} \
        if d.exists() else {}


def sig(f: dict, name: str) -> str:
    args = ", ".join(decl(t, f"a{i}") for i, t in enumerate(f["types"])) or "void"
    return f"{f['ret']} {name}({args})"


def call(f: dict, name: str) -> str:
    return f"{name}(" + ", ".join(f"a{i}" for i in range(len(f["types"]))) + ")"


# Structs that are a register set, not memory: a `conv` adapter takes a pointer to one as the
# registers to enter with (game/runtime/asm_conv.cpp).
REG_IMAGES = {"ScalerRegs"}

SCALAR_PTR = re.compile(r"((?:(?:signed|unsigned)\s+)?(?:short|long|int)(?:\s+int)?)\s*\*")


def slot_arg(t: str, i: int) -> str:
    """Argument i of a call from generated code, as the C function's parameter type t. A host
    scalar pointer gets a host copy (host_scalar); the temporary lives until the call returns."""
    m = SCALAR_PTR.fullmatch(t.strip())
    if m:
        return f"host_scalar<{m.group(1)}>(xm_slot_arg({i})).p()"
    return f"from_slot<{t}>(xm_slot_arg({i}))"


def norm_type(t: str) -> str:
    return re.sub(r"\s*\*\s*", "*", re.sub(r"\s+", " ", t.strip()))


def definition_fits(p: Path, f: dict) -> bool:
    """Whether the C file defines f with the prototype's types. A file caught mid-edit (its
    definition already changed, the header not yet, or the other way round) would otherwise
    fail to link (C++ overloads)."""
    text = re.sub(r"/\*.*?\*/", " ", p.read_text(encoding="utf-8", errors="replace"), flags=re.S)
    m = re.search(r"^([^\n;#{}]*?)\b" + re.escape(f["name"]) + r"\s*\(([^;{]*)\)\s*\{", text, re.M)
    if not m:
        return False
    ret = norm_type(re.sub(r"\b(static|register)\b", "", m.group(1)))
    params = m.group(2).strip()
    plist = [] if params in ("", "void") else split_params(params)
    types = []
    for q in plist:
        q = re.sub(r"\bregister\b", "", q).strip()
        if re.search(r"\(\s*\*\s*[A-Za-z_]\w*\s*\)\s*\(", q):
            types.append(re.sub(r"\(\s*\*\s*[A-Za-z_]\w*\s*\)", "(*)", q))
        else:
            types.append(re.sub(r"\b[A-Za-z_]\w*$", "", q).strip() if re.search(r"[\s*][A-Za-z_]\w*$", q) and
                         not re.fullmatch(r"(unsigned\s+|signed\s+)?(char|short|int|long)", q) else q)
    return ret == norm_type(f["ret"]) and [norm_type(t) for t in types] == [norm_type(t) for t in f["types"]]


def fn_table(protos: list[dict], wraps: list[dict]) -> list[str]:
    """The function-address table: host function (the C-visible name) <-> 68000 address, and
    the value each function's address had where a C function took it (xmen2.fnrefs.csv):
    xm::fn_value / fn_host / fn_enter / fn_leave."""
    L = []
    L += ["", "struct FnEntry { const void *host; uint32_t addr; };",
          "static const FnEntry s_fns[] = {"]
    for f in protos:
        if not f["fragment"]:
            L.append(f"    {{ (const void *)&{f['name']}, 0x{f['addr']:06X}u }},")
    # inside its own file a C function's name is hc_<Name> (-D<Name>=hc_<Name>): a function that
    # stores its own address (a state re-arming itself) takes that one
    for f in wraps:
        L.append(f"    {{ (const void *)&hc_{f['name']}, 0x{f['addr']:06X}u }},")
    L += ["};", "struct FnRef { uint32_t fn, target, value; };", "static const FnRef s_refs[] = {"]
    with open(common.GAME / "xmen2.fnrefs.csv", newline="") as fh:
        for r in csv.DictReader(fh):
            L.append(f"    {{ 0x{r['function']}u, 0x{r['target']}u, 0x{r['value']}u }},")
    L += ["};",
          "namespace xm {",
          "static uint32_t s_cur_fn;   /* the C function running (the one whose references apply) */",
          "void fn_enter(uint32_t fn, uint32_t *saved) { *saved = s_cur_fn; s_cur_fn = fn; }",
          "void fn_leave(uint32_t saved) { s_cur_fn = saved; }",
          "uint32_t fn_current(void) { return s_cur_fn; }",
          "/* Every game function has its own host address: two at one means the linker folded",
          "   identical code (CMakeLists.txt links with /OPT:NOICF), and a stored state would come",
          "   back as the other function. Checked once. */",
          "static void fn_check(void)",
          "{",
          "    static bool done;",
          "    if (done) return;",
          "    done = true;",
          "    const size_t n = sizeof s_fns / sizeof s_fns[0];",
          "    for (size_t i = 0; i < n; i++)",
          "        for (size_t j = i + 1; j < n; j++)",
          "            if (s_fns[i].host == s_fns[j].host && s_fns[i].addr != s_fns[j].addr)",
          "                fatal(\"xm: $%06X and $%06X share one host address (identical code folded)\",",
          "                      (unsigned)s_fns[i].addr, (unsigned)s_fns[j].addr);",
          "}",
          "static uint32_t fn_addr(const void *host)",
          "{",
          "    fn_check();",
          "    for (const FnEntry &e : s_fns) if (e.host == host) return e.addr;",
          "    fatal(\"xm::fn_value: %p is not a game function\", host);",
          "}",
          "uint32_t fn_value(const void *host)",
          "{",
          "    uint32_t t = fn_addr(host);",
          "    for (const FnRef &r : s_refs) if (r.fn == s_cur_fn && r.target == t) return r.value;",
          "    fatal(\"xm::fn_value: $%06X takes the address of $%06X, which the original never did\","
          " (unsigned)s_cur_fn, (unsigned)t);",
          "}",
          "const void *fn_host(uint32_t a)",
          "{",
          "    fn_check();",
          "    for (const FnEntry &e : s_fns) if (e.addr == (a & 0xFFFFFFu)) return e.host;",
          "    return nullptr;",
          "}",
          "}  /* namespace xm */", ""]
    return L


def native_calls(wraps: list[dict]) -> list[str]:
    """XMen2Native (phase 5 P3, xmen2_mem.h NATIVE): a call through a function value stored in game
    memory (fnp, AnyFn) reaches the C function at that value with no 68000 in between. For each
    function in C a thunk takes the arguments as narg (xmen2_mem.h make_narg) and converts them per
    parameter type, as its slot entry does from the 68000 stack slots; the result goes to xm::g_d0
    with the slot entry's width (a narrower result keeps d0's upper bytes, as move.w/move.b do), and
    the caller reads d0. A pointer to the caller's host memory arrives as itself. call_native finds
    the thunk by the value (through jt `jmp` thunks and AI-script `jmp d(a5)` steps, as
    call_slots does) and moves the stack region by what call68 would have reserved (the copies
    of host-pointer arguments, the slots, the return address), so that the escaping locals placed
    there (xm::frame_alloc) keep the original's addresses. A value with no C function (a fragment,
    an interior label) stops the run: no generated code can take it."""
    L = ["#ifdef XMEN2_NATIVE",
         "template <class T> static inline T from_narg(const xm::narg &a)",
         "{",
         "    if constexpr (std::is_pointer<T>::value && !std::is_function<typename std::remove_pointer<T>::type>::value)",
         "        if (a.p) return (T)a.p;   /* the caller's host memory */",
         "    return from_slot<T>(a.v);",
         "}",
         "/* a plain scalar pointer parameter: the caller's host value itself, or (a game-memory address)",
         "   a host copy of the big-endian value, written back after the call (host_scalar) */",
         "template <class T> struct narg_scalar {",
         "    host_scalar<T> hs; T *direct;",
         "    explicit narg_scalar(const xm::narg &a) : hs(a.p ? 0u : a.v), direct((T *)a.p) {}",
         "    T *p() { return direct ? direct : hs.p(); }",
         "};",
         "namespace xm {",
         "uint32_t g_d0;   /* d0 as the 68000 code of a stored-pointer call would leave it */",
         "static inline void set_d0(uint32_t v, int bytes)",
         "{",
         "    if (bytes >= 4)      g_d0 = v;",
         "    else if (bytes == 2) g_d0 = (g_d0 & 0xFFFF0000u) | (v & 0xFFFFu);",
         "    else if (bytes == 1) g_d0 = (g_d0 & 0xFFFFFF00u) | (v & 0xFFu);",
         "}",
         "uint32_t frame_mark(void);",
         "void frame_release(uint32_t mark);",
         "}  /* namespace xm */",
         "typedef uint32_t (*NThunkFn)(const xm::narg *a);",
         "struct NThunk { uint32_t addr; int nargs; NThunkFn fn; };"]
    rows = []
    for f in sorted(wraps, key=lambda f: f["addr"]):
        a = f["addr"]
        n = len(f["types"])
        L.append(f"static uint32_t nthunk_{a:06X}(const xm::narg *a)")
        L.append("{")
        if f["conv"] or f["regs"] is not None or f["noreturn"] or f["vector"]:
            L.append(f'    (void)a; xm::fatal("a stored pointer calls ${a:06X} {f["name"]}, which takes no stack call");')
        else:
            args = []
            for i, t in enumerate(f["types"]):
                m = SCALAR_PTR.fullmatch(t.strip())
                if m:
                    L.append(f"    narg_scalar<{m.group(1)}> s{i}(a[{i}]);")
                    args.append(f"s{i}.p()")
                else:
                    args.append(f"from_narg<{t}>(a[{i}])")
            callx = f"{f['name']}({', '.join(args)})"
            if f["ret"] == "void":
                L.append(f"    {callx};")
            else:
                L.append(f"    {f['ret']} r = {callx};")
                L.append(f"    xm::set_d0(to_u32(r), val_bytes<{f['ret']}>());")
        L.append("    return xm::g_d0;")
        L.append("}")
        rows.append(f"    {{ 0x{a:06X}u, {n}, nthunk_{a:06X} }},")
    L += ["static const NThunk s_nthunks[] = {", *rows, "};",
          "namespace xm {",
          "static inline uint16_t rom16(uint32_t a) { return (uint16_t)((g_rom[a] << 8) | g_rom[a + 1]); }",
          "uint64_t g_native_calls, g_native_short_calls;",
          "uint32_t call_native(uint32_t a, const narg *args, int n)",
          "{",
          "    a &= 0xFFFFFFu;",
          "    for (int i = 0; i < 4 && a + 6u <= 0x400000u; i++) {   /* through jumps, as call_slots */",
          "        uint16_t op = rom16(a);",
          "        if (op == 0x4EED) a = (0x200u + (uint32_t)(int32_t)(int16_t)rom16(a + 2)) & 0xFFFFFFu;",
          "        else if (op == 0x4EF9) a = (((uint32_t)rom16(a + 2) << 16) | rom16(a + 4)) & 0xFFFFFFu;",
          "        else break;",
          "    }",
          "    size_t lo = 0, hi = sizeof s_nthunks / sizeof s_nthunks[0];",
          "    while (lo < hi) {",
          "        size_t mid = (lo + hi) / 2;",
          "        if (s_nthunks[mid].addr < a) lo = mid + 1; else hi = mid;",
          "    }",
          "    if (lo == sizeof s_nthunks / sizeof s_nthunks[0] || s_nthunks[lo].addr != a)",
          "        fatal(\"call of $%06X through a stored pointer: no C function starts there\", (unsigned)a);",
          "    const NThunk &t = s_nthunks[lo];",
          "    narg pad[16] = {};",
          "    if (n < t.nargs) {   /* old-style: fewer arguments than the prototype has (the rest read 0) */",
          "        if (t.nargs > 16) fatal(\"call_native: $%06X has %d parameters\", (unsigned)a, t.nargs);",
          "        for (int i = 0; i < n; i++) pad[i] = args[i];",
          "        args = pad;",
          "        if (!g_native_short_calls++)",
          "            fprintf(stderr, \"[native] $%06X called through a stored pointer with %d of its %d arguments\\n\",",
          "                    (unsigned)a, n, t.nargs);",
          "    }",
          "    uint32_t mark = frame_mark(), res = 4u * (uint32_t)n + 4u;",
          "    for (int i = 0; i < n; i++) res += args[i].stack;",
          "    frame_release(mark - res);",
          "    g_native_calls++;",
          "    uint32_t d0 = t.fn(args);",
          "    frame_release(mark);",
          "    return d0;",
          "}",
          "}  /* namespace xm */",
          "#endif  /* XMEN2_NATIVE */", ""]
    return L


def generate(out: Path, mode: str) -> None:
    protos = parse_protos()
    by_addr = {f["addr"]: f for f in protos}
    hc = hand_c()
    unfit = [a for a, p in hc.items() if a in by_addr and not definition_fits(p, by_addr[a])]
    for a in unfit:
        print(f"bridge: game/c/{a:06X}.c left out: its definition doesn't fit the prototype of "
              f"{by_addr[a]['name']} in xmen2_funcs.h (being edited?)", file=sys.stderr)
        del hc[a]
    missing = [a for a in hc if a not in by_addr]
    if missing:
        sys.exit(f"game/c/{missing[0]:06X}.c: no prototype for ${missing[0]:06X} in xmen2_funcs.h")
    L = ['/* xmen2_bridge.cpp - GENERATED by tools/bridge.py (mode %s). Do not edit. */' % mode,
         '#include <stdio.h>',
         '#include <stdlib.h>',
         f'#include "{(headers_dir() / "xmen2.h").as_posix()}"' if SRC else '#include "xmen2.h"',
         '',
         'extern "C" {',
         'int x68k_entry(uint32_t addr);',
         'void xm_slot_return(void);',
         'uint32_t xm_slot_arg(int i);',
         'void xm_set_d0(uint32_t v, int bytes);',
         'uint32_t xm_reg(int r);',
         'void xm_set_reg(int r, uint32_t v, int bytes);',
         'void xm_regs_save(uint32_t *saved);',
         'void xm_regs_restore(const uint32_t *saved);',
         '}',
         'namespace xm { void fn_enter(uint32_t fn, uint32_t *saved); void fn_leave(uint32_t saved); }',
         '',
         'template <class T> static inline T from_slot(uint32_t s)',
         '{',
         '    if constexpr (std::is_pointer<T>::value && std::is_function<typename std::remove_pointer<T>::type>::value) {',
         '        const void *h = xm::fn_host(s);   /* a C-visible function: its wrapper or stub */',
         '        if (!h && s) xm::fatal("bridge: $%06X is not a game function", (unsigned)s);',
         '        return (T)h;',
         '    }',
         '    else if constexpr (std::is_pointer<T>::value) return (T)xm::host(s);',
         '    else if constexpr (std::is_same<T, bool>::value) return s != 0;',
         '    else if constexpr (std::is_class<T>::value) { T r{}; r.v = s; return r; }   /* fnp<>, bptr<> */',
         '    else return (T)(typename std::make_signed<T>::type)(int32_t)s;',
         '}',
         'template <class R, class... A> static constexpr int ret_bytes(R (*)(A...))',
         '{',
         '    if constexpr (std::is_void<R>::value) return 0;',
         '    else if constexpr (std::is_pointer<R>::value) return 4;',
         '    else return (int)sizeof(R);',
         '}',
         '/* Run one C function call, numbered so NO_RETURN_VALUE marks this call: xm::g_retnv',
         '   says, right after, whether it ended where the original returns no value. */',
         'template <class F> static inline auto call_c(F f)',
         '{',
         '    uint32_t saved = xm::g_frame, me = xm::g_frame = ++xm::g_seq;',
         '    if constexpr (std::is_void<decltype(f())>::value) {',
         '        f();',
         '        xm::g_retnv = false;',
         '        xm::g_frame = saved;',
         '    } else {',
         '        auto r = f();',
         '        xm::g_retnv = xm::g_novalue == me;',
         '        xm::g_frame = saved;',
         '        return r;',
         '    }',
         '}',
         'template <class T> static inline void to_d0(T v)',
         '{',
         '    if constexpr (std::is_pointer<T>::value) xm_set_d0(xm::addr((const void *)v), 4);',
         '    else xm_set_d0((uint32_t)(int32_t)v, (int)sizeof(T) > 4 ? 4 : (int)sizeof(T));',
         '}',
         'template <class T> static inline uint32_t to_u32(T v)',
         '{',
         '    if constexpr (std::is_pointer<T>::value) return xm::addr((const void *)v);',
         '    else return (uint32_t)(int32_t)v;',
         '}',
         'template <class T> static constexpr int val_bytes()',
         '{',
         '    if constexpr (std::is_pointer<T>::value) return 4;',
         '    else return (int)sizeof(T) > 4 ? 4 : (int)sizeof(T);',
         '}',
         '/* A plain `short *`, `long *` or `int *` parameter points at a host value in the C (a caller\'s local),',
         '   but from generated code it is a 68000 address of a big-endian one. The C gets a host copy,',
         '   written back when the call returns: the reverse of call68\'s Scratch. */',
         'template <class T> struct host_scalar {',
         '    uint32_t a; T v;',
         '    explicit host_scalar(uint32_t s) : a(s), v(0)',
         '    {',
         '        if (s) v = (T)(sizeof(T) == 2 ? (uint32_t)m68k_read16(s) : m68k_read32(s));',
         '    }',
         '    ~host_scalar()',
         '    {',
         '        if (!a) return;',
         '        if (sizeof(T) == 2) m68k_write16(a, (uint16_t)v); else m68k_write32(a, (uint32_t)v);',
         '    }',
         '    T *p() { return a ? &v : nullptr; }',
         '};',
         '']
    stubs, wraps = [], []
    for f in protos:
        if f["fragment"]:
            continue
        a = f["addr"]
        ret_void = f["ret"] == "void"
        odd = f["conv"] or f["regs"] is not None
        if f["conv"]:
            # an odd convention (stack or SR effects): the adapters are hand-written
            # (game/runtime/asm_conv.cpp): conv_call_ADDR calls the generated code, conv_slot_ADDR
            # takes a call from generated code and runs the C it is given
            L.append(f"{sig(f, f'conv_call_{a:06X}')};")
        elif f["regs"] is not None:
            L += regs_call(f)
        if a in hc:
            wraps.append(f)
            argv = ", ".join(f"a{i}" for i in range(len(f["types"])))
            L.append(f"{sig(f, 'hc_' + f['name'])};")
            L.append(f"{sig(f, f['name'])}")
            L.append("{")
            if mode == "verify":
                # a shadow test runs the C of ONE function; what it calls runs as generated code,
                # except with a pointer to a host local: generated code can't reach it, and a copy
                # on the 68000 stack has no length to go by (a `void *`; an array such as
                # Obj_CollideSegs' contact list, which Seg_GatherNearObj fills past its first
                # element), so the callee's C runs. Register out-parameters (&dN) go by register,
                # and so does a register image (REG_IMAGES: Scaler_RowBlocks' adapter loads the
                # host ScalerRegs into the registers, and they stay as the loop leaves them).
                hostp = [f"(a{i} && !xm::in_game_memory((const void *)a{i}))" for i, t in enumerate(f["types"])
                         if t.strip().endswith("*") and "(" not in t and pointee(t) not in REG_IMAGES
                         and not (f["regs"] is not None and f["regs"][i].startswith("&"))]
                if hostp:
                    # entered like any C function (fn_enter: the function references that apply
                    # are its own; Hornet_SpawnForDeathbird's C stores Hornet_MoveVelocity)
                    hc_call = f"hc_{f['name']}({argv})"
                    L.append(f"    if ({' || '.join(hostp)}) {{")
                    L.append(f"        uint32_t saved; xm::fn_enter(0x{a:06X}u, &saved);")
                    if ret_void:
                        L.append(f"        {hc_call};")
                        L.append("        xm::fn_leave(saved);")
                        L.append("        return;")
                    else:
                        L.append(f"        {f['ret']} r = call_c([&] {{ return {hc_call}; }});")
                        L.append("        xm::fn_leave(saved);")
                        L.append("        return r;")
                    L.append("    }")
                if odd:
                    c = f"{'conv' if f['conv'] else 'regs'}_call_{a:06X}({argv})"
                    L.append(f"    {c};" if ret_void else f"    return {c};")
                else:
                    c = f"xm::call68(0x{a:06X}u{', ' + argv if argv else ''})"
                    L.append(f"    {c};" if ret_void else f"    return ({f['ret']}){c};")
            else:
                L.append(f"    uint32_t saved; x68k_entry(0x{a:06X}u); xm::fn_enter(0x{a:06X}u, &saved);")
                if ret_void:
                    L.append(f"    {call(f, 'hc_' + f['name'])};")
                    L.append("    xm::fn_leave(saved);")
                else:
                    L.append(f"    {f['ret']} r = call_c([&] {{ return {call(f, 'hc_' + f['name'])}; }});")
                    L.append("    xm::fn_leave(saved);")
                    L.append("    return r;")
            L.append("}")
            # slot entry: a call from generated code (68000 stack: return address, then the slots)
            target = f["name"] if mode != "verify" else "hc_" + f["name"]
            if f["conv"]:
                L.append(f"void conv_slot_{a:06X}({sig(f, '(*c)')});")
                L.append(f"extern \"C\" void hcslot_{a:06X}(void)")
                L.append("{")
                if mode == "verify":
                    L.append(f"    uint32_t saved; xm::fn_enter(0x{a:06X}u, &saved);")
                L.append(f"    conv_slot_{a:06X}({target});")
                if mode == "verify":
                    L.append("    xm::fn_leave(saved);")
                L.append("}")
                if mode in ("stepped", "handc"):
                    L.append(f"extern \"C\" void func_{a:06X}(void) {{ hcslot_{a:06X}(); }}")
                L.append("")
                continue
            L.append(f"extern \"C\" void hcslot_{a:06X}(void)")
            L.append("{")
            args = ", ".join(slot_arg(t, i) for i, t in enumerate(f["types"]))
            if mode == "verify":
                L.append(f"    uint32_t saved; xm::fn_enter(0x{a:06X}u, &saved);")
            if f["regs"] is not None:
                L += regs_slot(f, target, mode == "verify")
            elif ret_void:
                L.append(f"    {target}({args});")
                L.append("    xm::g_retnv = false;")
            elif mode == "verify":
                L.append(f"    auto r = call_c([&] {{ return {target}({args}); }});")
                L.append("    if (!xm::g_retnv) to_d0(r);")
            else:
                L.append(f"    auto r = {target}({args});")
                L.append("    if (!xm::g_retnv) to_d0(r);")
            if mode == "verify":
                L.append("    xm::fn_leave(saved);")
            L.append("    xm_slot_return();")
            L.append("}")
            if mode in ("stepped", "handc"):
                L.append(f"extern \"C\" void func_{a:06X}(void) {{ hcslot_{a:06X}(); }}")
            L.append("")
        else:
            stubs.append(f)
            L.append(f"{sig(f, f['name'])}")
            L.append("{")
            argv = ", ".join(f"a{i}" for i in range(len(f["types"])))
            if odd:
                c = f"{'conv' if f['conv'] else 'regs'}_call_{a:06X}({argv})"
                L.append(f"    {c};" if ret_void else f"    return {c};")
            else:
                c = f"xm::call68(0x{a:06X}u{', ' + argv if argv else ''})"
                L.append(f"    {c};" if ret_void else f"    return ({f['ret']}){c};")
            L.append("}")
    # the shadow-test harness's view (game/runtime/verify.cpp): every function in C
    ends = {}
    with open(common.GAME / "xmen2.functions.csv", newline="") as fh:
        for r in csv.DictReader(fh):
            ends[int(r["addr"], 16)] = int(r["end"], 16)
    # kind 1 = asm (hand-written 68000), 2 = asm with hand-written adapters (conv): verify.cpp
    # compares their results by kind; out_* = the registers a register convention hands back
    # (as wide as their C type), skip_mask = clobbers(); arg_bytes = the function's own argument
    # slots above its return address (stack convention only), dead stack for the comparison
    # (P4-decomp.md decision 9)
    L += ["", "struct XmVerifyFn { uint32_t addr, end; void (*slot)(void); void (*gen)(void); int ret_bytes;",
          "                    int kind; uint16_t out_mask, skip_mask; uint8_t out_bytes[16]; int arg_bytes; };"]
    if mode == "verify":
        L += [f'extern "C" void func_{f["addr"]:06X}(void);' for f in wraps]
    L.append("extern const XmVerifyFn xm_verify_fns[] = {")
    # a function that never returns (the reset entry) or that an interrupt enters and rte leaves
    # (the interrupt vectors) can't be shadow-tested: verify runs a call to its return. The
    # whole-game traces (D2, D6) cover them.
    tested = [f for f in wraps if not f["noreturn"] and not f["vector"]]
    for f in tested:
        gen = f"func_{f['addr']:06X}" if mode == "verify" else "nullptr"
        o = outs(f)
        ob = ["0"] * 16
        for r, t in o:
            ob[r] = f"val_bytes<{t}>()"
        rb = "0" if f["regs"] is not None else f"ret_bytes(&hc_{f['name']})"
        kind = 2 if f["conv"] else 1 if f["asm"] else 0
        argb = 0 if f["conv"] or f["regs"] is not None else 4 * len(f["types"])
        L.append(f"    {{ 0x{f['addr']:06X}u, 0x{ends.get(f['addr'], f['addr']):06X}u, hcslot_{f['addr']:06X}, {gen}, "
                 f"{rb}, {kind}, 0x{sum(1 << r for r, _ in o):04X}u, "
                 f"0x{sum(1 << regno(r) for r in f['clobbers']):04X}u, {{ {', '.join(ob)} }}, {argb} }},")
    L += ["    { 0, 0, nullptr, nullptr, 0, 0, 0, 0, { 0 } }", "};",
          f"extern const int xm_verify_fn_count = {len(tested)};"]
    L += fn_table(protos, wraps)
    if mode == "stepped":
        L += native_calls(wraps)
    skip = set(hc)
    if mode == "stepped":
        # Fragments (switch cases and blocks split off a function, AI-script jmp tables) are done
        # with their parent (P4-decomp.md decision 7): when the function containing one is in C,
        # its generated code goes too. The C reaches a jmp table only by calling into it, and the
        # runtime follows such a call to the jump's target (handc_rt.cpp, through_jumps). The
        # dispatch table still names the fragment: its func_ stops loudly if anything reaches it.
        starts = sorted(f["addr"] for f in protos if not f["fragment"])
        for f in protos:
            if f["fragment"]:
                i = bisect.bisect_right(starts, f["addr"]) - 1
                if i >= 0 and starts[i] in hc:
                    skip.add(f["addr"])
                    L.append(f'extern "C" void func_{f["addr"]:06X}(void) {{ xm::fatal("dispatch to ${f["addr"]:06X}, '
                             f'a fragment of a function written in C"); }}')
    out.mkdir(parents=True, exist_ok=True)
    write_if_changed(out / "xmen2_bridge.cpp", "\n".join(L))
    # One wrapper per C file (handc/<ADDR>.cpp: the renaming defines, then the file), all with
    # the same compile options: MSBuild hands them to one cl call, which /MP spreads over the
    # cores. Per-file COMPILE_DEFINITIONS made every file its own cl call, one at a time.
    wd = out / "handc"
    wd.mkdir(parents=True, exist_ok=True)
    wrappers = []
    for a, p in hc.items():
        f = by_addr[a]
        w = wd / f"{a:06X}.cpp"
        write_if_changed(w, f"/* GENERATED by tools/bridge.py: {p.name} as hc_{f['name']}. Do not edit. */\n"
                            f"#define {f['name']} hc_{f['name']}\n"
                            f"#define XMEN2_FN 0x{a:06X}\n"
                            f'#include "{p.as_posix()}"\n')
        wrappers.append(w)
    cm = ["# GENERATED by tools/bridge.py. Do not edit.", "set(XMEN2_HANDC_SOURCES"]
    cm += [f'    "{w.as_posix()}"' for w in wrappers]
    cm.append(")")
    write_if_changed(out / "handc.cmake", "\n".join(cm) + "\n")
    write_if_changed(out / "handc_skip.txt", "".join(f"{a:06X}\n" for a in sorted(skip)))
    print(f"bridge ({mode}): {len(wraps)} functions in C, {len(stubs)} stubs to generated code -> {out}")


# The sync point (xmen2disasm P4-decomp.md: every main-program entry to it; game/xmen2_spec.c
# XMEN2_SYNC_PC): in the native runner its wrapper calls the platform's frame step.
SYNC_FN = "VDP_WaitVBlank"


def generate_native(out: Path) -> None:
    """XMen2Native's bridge (phase 5 P6): no 68000 behind the C at all. Every function is C; its
    C-visible name is a plain wrapper (the function-reference context, xm::fn_enter, and at the
    sync point the platform's frame step), the function-address table, and the typed thunks of
    calls through stored pointers (native_calls). No slot entries, no dispatch, no generated code.
    The per-file wrappers (handc/, handc.cmake) are the stepped bridge's."""
    protos = parse_protos()
    by_addr = {f["addr"]: f for f in protos}
    hc = hand_c()
    unfit = [a for a, p in hc.items() if a in by_addr and not definition_fits(p, by_addr[a])]
    if unfit:
        sys.exit(f"bridge (native): game/c/{unfit[0]:06X}.c doesn't fit its prototype in xmen2_funcs.h")
    wraps = [f for f in protos if not f["fragment"] and f["addr"] in hc]
    lacking = [f for f in protos if not f["fragment"] and f["addr"] not in hc]
    if lacking:
        sys.exit(f"bridge (native): ${lacking[0]['addr']:06X} {lacking[0]['name']} has no C (the native runner "
                 f"has no generated code; {len(lacking)} functions lack C)")
    if not any(f["name"] == SYNC_FN for f in wraps):
        sys.exit(f"bridge (native): no {SYNC_FN} in C")
    L = ['/* xmen2_bridge.cpp - GENERATED by tools/bridge.py (mode native). Do not edit. */',
         '#include <stdio.h>',
         '#include <stdlib.h>',
         f'#include "{(headers_dir() / "xmen2.h").as_posix()}"' if SRC else '#include "xmen2.h"',
         '',
         'extern "C" void plat_sync_point(void);   /* the platform\'s frame step (platform/native) */',
         'namespace xm { void fn_enter(uint32_t fn, uint32_t *saved); void fn_leave(uint32_t saved); }',
         '',
         'template <class T> static inline T from_slot(uint32_t s)',
         '{',
         '    if constexpr (std::is_pointer<T>::value && std::is_function<typename std::remove_pointer<T>::type>::value) {',
         '        const void *h = xm::fn_host(s);   /* a C-visible function: its wrapper */',
         '        if (!h && s) xm::fatal("bridge: $%06X is not a game function", (unsigned)s);',
         '        return (T)h;',
         '    }',
         '    else if constexpr (std::is_pointer<T>::value) return (T)xm::host(s);',
         '    else if constexpr (std::is_same<T, bool>::value) return s != 0;',
         '    else if constexpr (std::is_class<T>::value) { T r{}; r.v = s; return r; }   /* fnp<>, bptr<> */',
         '    else return (T)(typename std::make_signed<T>::type)(int32_t)s;',
         '}',
         'template <class T> static inline uint32_t to_u32(T v)',
         '{',
         '    if constexpr (std::is_pointer<T>::value) return xm::addr((const void *)v);',
         '    else return (uint32_t)(int32_t)v;',
         '}',
         'template <class T> static constexpr int val_bytes()',
         '{',
         '    if constexpr (std::is_pointer<T>::value) return 4;',
         '    else return (int)sizeof(T) > 4 ? 4 : (int)sizeof(T);',
         '}',
         '/* A plain `short *` or `long *` parameter given a game-memory address: a host copy of the',
         '   value there, written back when the call returns (game memory through its own types). */',
         'template <class T> struct host_scalar {',
         '    uint32_t a; T v;',
         '    explicit host_scalar(uint32_t s) : a(s), v(0)',
         '    {',
         '        if (!s) return;',
         '        if (sizeof(T) == 2) v = (T)(uint16_t)*(const xm::be<uint16_t> *)xm::host(s);',
         '        else v = (T)(uint32_t)*(const xm::be<uint32_t> *)xm::host(s);',
         '    }',
         '    ~host_scalar()',
         '    {',
         '        if (!a) return;',
         '        if (sizeof(T) == 2) *(xm::be<uint16_t> *)xm::host(a) = (uint16_t)v;',
         '        else *(xm::be<uint32_t> *)xm::host(a) = (uint32_t)v;',
         '    }',
         '    T *p() { return a ? &v : nullptr; }',
         '};',
         '']
    for f in wraps:
        a = f["addr"]
        L.append(f"{sig(f, 'hc_' + f['name'])};")
        L.append(sig(f, f["name"]))
        L.append("{")
        if f["name"] == SYNC_FN:
            L.append("    plat_sync_point();")
        L.append(f"    uint32_t saved; xm::fn_enter(0x{a:06X}u, &saved);")
        if f["ret"] == "void":
            L.append(f"    {call(f, 'hc_' + f['name'])};")
            L.append("    xm::fn_leave(saved);")
        else:
            L.append(f"    {f['ret']} r = {call(f, 'hc_' + f['name'])};")
            L.append("    xm::fn_leave(saved);")
            L.append("    return r;")
        L.append("}")
    L += fn_table(protos, wraps)
    L += native_calls(wraps)
    out.mkdir(parents=True, exist_ok=True)
    write_if_changed(out / "xmen2_bridge.cpp", "\n".join(L))
    print(f"bridge (native): {len(wraps)} functions in C -> {out}")


def main() -> int:
    global SRC
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", type=Path, default=common.BUILD / "bridge")
    ap.add_argument("--mode", default="stepped", choices=["stepped", "handc", "verify", "native"])
    ap.add_argument("--src", type=Path, help="a snapshot of game/c and game/include in one directory")
    a = ap.parse_args()
    SRC = a.src.resolve() if a.src else None
    if a.mode == "native":
        generate_native(a.out)
    else:
        generate(a.out, a.mode)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
