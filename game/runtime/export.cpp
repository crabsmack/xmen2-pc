/* export.cpp - XMen2Native's state exporter (phase 5): the game state as the 68000 image the
 * references compare (xmen2_image.h), from wherever that state lives.
 *
 * Phase 5 moves the game's C onto native memory one piece at a time; equality with the
 * original is still decided by the phase-4 sync rows (stepped-gen references), so the native
 * runner computes its rows from this image only (xmen2_spec.c xmen2_on_sync):
 *   1. work RAM is first copied from the emulated 68000 RAM: every byte no native storage
 *      covers yet comes from there;
 *   2. then every global of xmen2_globals.h is written by name, address and declared type
 *      (the table tools/export_table.py generates from the header): put_value serializes a
 *      value in its 68000 form (big-endian, pointers as 68000 addresses). A global whose
 *      storage is still the emulated RAM writes the bytes the copy already holds; one moved to
 *      native storage (xmen2_mem.h NATIVE) is the only source of its bytes;
 *   3. VRAM, CRAM and VSRAM come from the video model (the engine's VDP) in the reference's
 *      formats.
 * The counts say where the image came from (xm_export_stats, in the runner's counters):
 * native_bytes = work-RAM bytes serialized from storage outside the emulated RAM (0 while every
 * global lives in g_ram), copied_bytes = the rest of work RAM, taken from the emulated RAM.
 *
 * Self-test (negative test of the exporter -> sync row path): XMEN2_EXPORT_SELFTEST=1 (or the
 * name of a global) zeroes the last byte of that global in the image, as if the export had
 * dropped it. The trace must then differ from its reference from the first sync point where
 * that byte is non-zero, and only in RAM. Default g_TextWinTop ($FFC5A8): on the TAS its low
 * byte is 0 through sync 4,528 and 5 from sync 4,529 (the first text window), and the TAS trace
 * with the self-test is equal through 4,528 and differs from 4,529 (docs/NOTES.md "Phase 5").
 */
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <type_traits>

extern "C" {
#include "genesis_runtime.h"
#include "video/genesis_machine.h"
}
#include "xmen2_image.h"
/* The table, generated from the snapshot of game/include the hand C is built with
   (tools/export_table.py; it includes that snapshot's xmen2.h, which comes after every system
   header: on an LP64 host it makes `long` 32 bits, so nothing below says `long`). */
#include "xmen2_export_globals.inc"

namespace {

void put_be(uint8_t *d, uint32_t v, int n)
{
    for (int i = n - 1; i >= 0; i--, v >>= 8) d[i] = (uint8_t)v;
}

template <class T> struct is_be : std::false_type {};
template <class U> struct is_be<xm::be<U>> : std::true_type {};
template <class T> struct is_stored_ptr : std::false_type {};     /* a 68000 address in game memory */
template <class U> struct is_stored_ptr<xm::bptr<U>> : std::true_type {};
template <class U> struct is_stored_ptr<xm::fnp<U>> : std::true_type {};

/* One value in its 68000 form at d (sizeof(T) bytes of the image). */
template <class T> void put_value(uint8_t *d, const T &v)
{
    if constexpr (is_be<T>::value) {
        put_be(d, (uint32_t)v.get(), (int)sizeof(T));
    } else if constexpr (is_stored_ptr<T>::value) {
        put_be(d, v.raw(), 4);
    } else if constexpr (std::is_integral<T>::value || std::is_enum<T>::value) {
        static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4, "put_value: a 1, 2 or 4 byte scalar");
        put_be(d, (uint32_t)v, (int)sizeof(T));   /* a native scalar: big-endian */
    } else {
        /* A record: in PC mode every game struct IS the 68000 layout (be<> fields, bptr<>), so its
           bytes are its 68000 form. A struct moved to native storage needs its own put_value. */
        static_assert(std::is_trivially_copyable<T>::value, "put_value: a record must be plain bytes");
        if (!xm::in_game_memory(&v))
            xm::fatal("export: a record at %p is native storage with no writer (export.cpp put_value)", (const void *)&v);
        memcpy(d, &v, sizeof(T));
    }
}

/* n elements of type T (the declaration's) from p; returns the storage, so the caller knows where
   the bytes came from. */
template <class T, class E> const void *put_n(uint8_t *ram, uint32_t addr, const E *p, uint32_t n)
{
    static_assert(sizeof(E) == sizeof(T), "export: a global's storage changed size; give it its own writer");
    uint8_t *d = ram + (addr & 0xFFFFu);
    for (uint32_t i = 0; i < n; i++) put_value(d + i * (uint32_t)sizeof(T), p[i]);
    return (const void *)p;
}

struct Global {
    const char *name;
    uint32_t addr;                         /* $FFxxxx: the declaration's address */
    uint32_t bytes;                        /* n * sizeof(declared type) */
    bool unsized;                          /* declared `T name[]`: to the next global */
    const void *(*put)(uint8_t *ram);      /* writes it into the image; returns its storage */
};

#define XG(T, name, a)        { #name, a, (uint32_t)sizeof(T), false, \
                                [](uint8_t *ram) { return put_n<T>(ram, a, &(name), 1u); } },
#define XA(T, name, a, n)     { #name, a, (uint32_t)(sizeof(T) * (n)), false, \
                                [](uint8_t *ram) { return put_n<T>(ram, a, (name), (uint32_t)(n)); } },
#define XU(T, name, a, bytes) { #name, a, (uint32_t)(sizeof(T) * ((bytes) / sizeof(T))), true, \
                                [](uint8_t *ram) { return put_n<T>(ram, a, (name), (uint32_t)((bytes) / sizeof(T))); } },
const Global s_globals[] = { XMEN2_EXPORT_GLOBALS };
#undef XG
#undef XA
#undef XU
const int s_nglobals = (int)(sizeof s_globals / sizeof s_globals[0]);
static_assert(sizeof s_globals / sizeof s_globals[0] == XMEN2_EXPORT_GLOBAL_COUNT, "export table");

struct Stats {
    uint64_t exports;
    uint32_t native_bytes, copied_bytes, video_bytes, global_bytes, globals_native, unsized;
} s_stats;

const Global *s_selftest;
bool s_selftest_read;

const Global *selftest_global(void)
{
    if (s_selftest_read) return s_selftest;
    s_selftest_read = true;
    const char *e = getenv("XMEN2_EXPORT_SELFTEST");
    if (!e || !*e || !strcmp(e, "0")) return nullptr;
    const char *want = strcmp(e, "1") ? e : "g_TextWinTop";
    for (const Global &g : s_globals)
        if (!strcmp(g.name, want)) {
            fprintf(stderr, "[export] SELF-TEST: the image's byte $%06X (last of %s) is zeroed at every sync point\n",
                    (unsigned)(g.addr + g.bytes - 1u), g.name);
            return s_selftest = &g;
        }
    xm::fatal("XMEN2_EXPORT_SELFTEST=%s: no such global in xmen2_globals.h", e);
}

uint8_t s_native_map[0x10000];   /* work-RAM bytes the last export wrote from native storage */
uint8_t s_global_map[0x10000];   /* ... that some global covers */

}  // namespace

extern "C" void xm_export(XmImage *im)
{
    /* 1. the emulated RAM: the bytes no native storage covers */
    memcpy(im->ram, g_ram, sizeof im->ram);
    /* 2. every global by its declaration */
    memset(s_native_map, 0, sizeof s_native_map);
    memset(s_global_map, 0, sizeof s_global_map);
    uint32_t nat_globals = 0, unsized = 0;
    for (const Global &g : s_globals) {
        const void *storage = g.put(im->ram);
        const bool native = !xm::in_game_memory(storage);
        uint32_t off = g.addr & 0xFFFFu, end = off + g.bytes > 0x10000u ? 0x10000u : off + g.bytes;
        memset(s_global_map + off, 1, end - off);
        if (native) {
            memset(s_native_map + off, 1, end - off);
            nat_globals++;
        }
        unsized += g.unsized;
    }
    /* 3. the video model */
    const GVDP *v = &g_machine.vdp;
    memcpy(im->vram, v->vram, sizeof im->vram);
    xmen2_words_be(im->cram, v->cram, XMEN2_IMAGE_CRAM_WORDS, 0x0EEE);
    xmen2_words_be(im->vsram, v->vsram, XMEN2_IMAGE_VSRAM_WORDS, 0x07FF);

    if (const Global *t = selftest_global()) im->ram[(t->addr + t->bytes - 1u) & 0xFFFFu] = 0;

    uint32_t nat = 0, glob = 0;
    for (uint32_t i = 0; i < 0x10000u; i++) {
        nat += s_native_map[i];
        glob += s_global_map[i];
    }
    s_stats.exports++;
    s_stats.native_bytes = nat;
    s_stats.copied_bytes = 0x10000u - nat;
    s_stats.video_bytes = (uint32_t)(sizeof im->vram + sizeof im->cram + sizeof im->vsram);
    s_stats.global_bytes = glob;
    s_stats.globals_native = nat_globals;
    s_stats.unsized = unsized;
}

extern "C" int xm_export_stats(char *buf, int n)
{
    const Global *t = selftest_global();
    char st[96] = "null";
    if (t) snprintf(st, sizeof st, "\"%s+%u\"", t->name, (unsigned)(t->bytes - 1u));
    return snprintf(buf, (size_t)n,
                    "{\"native_bytes\": %u, \"copied_bytes\": %u, \"video_bytes\": %u, \"globals\": %d, "
                    "\"globals_native\": %u, \"global_bytes\": %u, \"globals_unsized\": %u, \"exports\": %" PRIu64 ", "
                    "\"selftest\": %s}",
                    (unsigned)s_stats.native_bytes, (unsigned)s_stats.copied_bytes, (unsigned)s_stats.video_bytes,
                    s_nglobals, (unsigned)s_stats.globals_native, (unsigned)s_stats.global_bytes,
                    (unsigned)s_stats.unsized, s_stats.exports, st);
}
