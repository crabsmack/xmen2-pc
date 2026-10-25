/* state_place.cpp - the process layout state files depend on (xmen2check.py verify and trace-group:
 * a TAS run saves states, other processes play the runs that branch from it from those).
 *
 * A saved state holds the game fiber's raw stack: return addresses, frame and local pointers, the
 * saved registers. It is valid in another process only if every address in it means the same
 * there:
 *   - the executable at the same base: the harness's runner copies have DYNAMICBASE cleared
 *     (xmen2check.py fixed_base), so Windows loads them at their preferred base;
 *   - the fiber's stack and XMen2Native's arena at fixed places (state_place.h), statics in the
 *     executable, and nothing the saved frames hold live on the heap (verify.cpp keeps its tests
 *     and counts in static storage for this);
 *   - the same /GS cookie: a frame's cookie is the process's random __security_cookie XOR its
 *     stack pointer, checked when the frame returns. The entry point (xm_entry, /ENTRY) takes
 *     XMEN2_GS_COOKIE (hex) before the C runtime starts, which then keeps it; the harness gives one
 *     random value to every process of a group. Without it each process keeps its own random one.
 * xm_place_get records these, xm_place_mismatch compares them before a load. */
#include "state_place.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

#ifdef _MSC_VER
extern "C" uintptr_t __security_cookie;
extern "C" DWORD mainCRTStartup(LPVOID);

/* The runners' entry point (/ENTRY:xm_entry): XMEN2_GS_COOKIE as the /GS cookie, then the C
   runtime's own start. Nothing here may use the C runtime, nor have a cookie of its own. */
extern "C" __declspec(safebuffers) DWORD xm_entry(LPVOID arg)
{
    char v[24];
    DWORD n = GetEnvironmentVariableA("XMEN2_GS_COOKIE", v, (DWORD)sizeof v);
    if (n && n < sizeof v) {
        uint64_t c = 0;
        bool ok = true;
        for (DWORD i = 0; i < n && ok; i++) {
            char ch = v[i];
            int d = ch >= '0' && ch <= '9' ? ch - '0' : ch >= 'a' && ch <= 'f' ? ch - 'a' + 10
                  : ch >= 'A' && ch <= 'F' ? ch - 'A' + 10 : -1;
            if (d < 0) ok = false;
            c = c << 4 | (uint64_t)(d < 0 ? 0 : d);
        }
        c &= 0x0000FFFFFFFFFFFFull;   /* as the runtime's own: the top 16 bits clear */
        if (ok && c && c != 0x00002B992DDFA232ull) __security_cookie = (uintptr_t)c;   /* not the default: kept */
    }
    return mainCRTStartup(arg);
}
#endif

static uint64_t cookie_hash(void)
{
#ifdef _MSC_VER
    uint64_t h = 0xCBF29CE484222325ull, c = (uint64_t)__security_cookie;
    for (int i = 0; i < 8; i++) { h ^= (c >> (8 * i)) & 0xFF; h *= 0x100000001B3ull; }
    return h;
#else
    return 0;
#endif
}

extern "C" void xm_place_get(XmPlace *p, uintptr_t fiber_lo, uintptr_t fiber_top, uintptr_t arena)
{
    memset(p, 0, sizeof *p);
    const uint8_t *img = (const uint8_t *)GetModuleHandleW(nullptr);
    const IMAGE_NT_HEADERS *nt = (const IMAGE_NT_HEADERS *)(img + ((const IMAGE_DOS_HEADER *)img)->e_lfanew);
    p->build = (uint64_t)nt->FileHeader.TimeDateStamp | (uint64_t)nt->OptionalHeader.SizeOfImage << 32;
    p->image_base = (uint64_t)(uintptr_t)img;
    p->cookie = cookie_hash();
    p->fiber_lo = fiber_lo;
    p->fiber_top = fiber_top;
    p->arena = arena;
}

extern "C" void xm_place_scan(const uint8_t *bytes, size_t n, uint64_t at, const XmPlace *p, const char *what)
{
    const char *e = getenv("XMEN2_STATE_SCAN");
    if (!e || !*e || *e == '0') return;
    HMODULE mods[256];
    DWORD need = 0;
    EnumProcessModules(GetCurrentProcess(), mods, sizeof mods, &need);
    size_t nmod = need / sizeof mods[0] < 256 ? need / sizeof mods[0] : 256;
    MODULEINFO info[256];
    for (size_t i = 0; i < nmod; i++) GetModuleInformation(GetCurrentProcess(), mods[i], &info[i], sizeof info[i]);
    uint64_t image_end = p->image_base + (p->build >> 32);
    unsigned own = 0, stack = 0, arena = 0, other_mod = 0, priv = 0, low = 0, shown = 0;
    bool all = *e == '2';   /* also the words below 4 GB (mostly 68000 values: addresses, pointers into game memory) */
    for (size_t off = 0; off + 8 <= n; off += 8) {
        uint64_t v;
        memcpy(&v, bytes + off, 8);
        if (v < 0x10000 || v >= 0x800000000000ull) continue;
        if (v >= p->image_base && v < image_end) { own++; continue; }
        if (v >= XM_FIBER_BASE && v < p->fiber_top) { stack++; continue; }   /* the stack, its guard and its header */
        if (p->arena && v >= p->arena && v < p->arena + 0x20000) { arena++; continue; }
        const char *kind = nullptr;
        char name[MAX_PATH] = "";
        for (size_t i = 0; i < nmod && !kind; i++) {
            uint64_t b = (uint64_t)(uintptr_t)info[i].lpBaseOfDll;
            if (v >= b && v < b + info[i].SizeOfImage) {
                kind = "module";
                GetModuleBaseNameA(GetCurrentProcess(), mods[i], name, sizeof name);
            }
        }
        MEMORY_BASIC_INFORMATION mb;
        if (!kind && VirtualQuery((const void *)(uintptr_t)v, &mb, sizeof mb) && mb.State == MEM_COMMIT)
            kind = mb.Type == MEM_PRIVATE ? "private" : mb.Type == MEM_MAPPED ? "mapped" : "image";
        if (!kind) continue;
        if (!strcmp(kind, "module")) other_mod++;
        else if (v < 0x100000000ull) low++;
        else priv++;
        if ((all || v >= 0x100000000ull) && shown++ < 24)
            fprintf(stderr, "[state-scan] %s: word at %016llx = %016llx: %s %s\n", what, (unsigned long long)(at + off),
                    (unsigned long long)v, kind, name);
    }
    fprintf(stderr, "[state-scan] %s: %zu bytes; addresses: executable %u, stack %u, arena %u, other modules %u, "
                    "other memory %u (and %u below 4 GB: XMEN2_STATE_SCAN=2 lists them)\n", what, n, own, stack, arena,
            other_mod, priv, low);
}

#else   /* not Windows: no states across processes (the executable is position-independent) */

extern "C" void xm_place_get(XmPlace *p, uintptr_t fiber_lo, uintptr_t fiber_top, uintptr_t arena)
{
    memset(p, 0, sizeof *p);
    p->image_base = (uint64_t)(uintptr_t)&xm_place_get;
    p->fiber_lo = fiber_lo;
    p->fiber_top = fiber_top;
    p->arena = arena;
}

extern "C" void xm_place_scan(const uint8_t *, size_t, uint64_t, const XmPlace *, const char *) {}

#endif

extern "C" const char *xm_place_mismatch(const XmPlace *saved, const XmPlace *mine)
{
    if (saved->build != mine->build) return "another executable saved it";
    if (saved->image_base != mine->image_base)
        return "the executable is loaded elsewhere (a runner copy with DYNAMICBASE cleared: xmen2check.py fixed_base)";
    if (saved->cookie != mine->cookie) return "another /GS cookie (XMEN2_GS_COOKIE: the same in both processes)";
    if (saved->fiber_lo != mine->fiber_lo || saved->fiber_top != mine->fiber_top) return "the game's fiber stack is elsewhere";
    if (saved->arena != mine->arena) return "the arena is elsewhere";
    return nullptr;
}
