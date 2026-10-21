/* native_rt.cpp - the memory layer under the hand-written C in XMen2Native (phase 5): game memory
 * in native storage, with no 68000 anywhere (xmen2disasm docs/phases/P5-port.md R1 (b)).
 *
 *   The arena: the 64 KB of work RAM, laid out as the 68000's (same sizes and offsets), mapped
 *   twice, back to back: xm_arena is the RAM as $00FFxxxx, xm_arena_ffff (= xm_arena + $10000)
 *   the same memory as $FFFFxxxx. The original stores both forms of RAM pointers (xmen2_mem.h),
 *   so a host pointer into either view converts back to exactly the value the original had, and
 *   the two forms compare unequal as on the 68000.
 *   The stack region: the original kept some locals whose addresses go into game memory on its
 *   stack (xmen2_mem.h xm::frame_*); they live in the arena where the original's frame had them.
 *   The mark starts at the reset SSP (the ROM's first long) and moves as the original's A7 did
 *   around them: by what a call through a stored pointer pushed (bridge.py call_native) and by an
 *   interrupt's frame (platform/native).
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

extern "C" {
uint8_t *xm_arena;        /* work RAM as $00FFxxxx */
uint8_t *xm_arena_ffff;   /* the same memory as $FFFFxxxx: xm_arena + $10000 */
extern uint8_t g_rom[0x400000];
}

namespace xm {

char g_fatal_msg[512];

[[noreturn]] void fatal(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(g_fatal_msg, sizeof g_fatal_msg, fmt, ap);
    va_end(ap);
    fprintf(stderr, "[native] %s\n", g_fatal_msg);
    fflush(stderr);
    exit(5);
}

/* ---- the arena ------------------------------------------------------------------------------ */

static void *map_twice(void)
{
#ifdef _WIN32
    HANDLE h = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 0x10000, nullptr);
    if (!h) return nullptr;
    for (int attempt = 0; attempt < 64; attempt++) {
        /* find 128 KB of free address space, then map the section into both halves (another
           thread could take the range in between: try again) */
        void *base = VirtualAlloc(nullptr, 0x20000, MEM_RESERVE, PAGE_NOACCESS);
        if (!base) break;
        VirtualFree(base, 0, MEM_RELEASE);
        void *a = MapViewOfFileEx(h, FILE_MAP_ALL_ACCESS, 0, 0, 0x10000, base);
        if (!a) continue;
        void *b = MapViewOfFileEx(h, FILE_MAP_ALL_ACCESS, 0, 0, 0x10000, (uint8_t *)base + 0x10000);
        if (b) return a;
        UnmapViewOfFile(a);
    }
    return nullptr;
#else
    char name[64];
    snprintf(name, sizeof name, "/xmen2-arena-%ld", (long)getpid());
    int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd < 0) return nullptr;
    shm_unlink(name);
    if (ftruncate(fd, 0x10000) != 0) { close(fd); return nullptr; }
    void *base = mmap(nullptr, 0x20000, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (base == MAP_FAILED) { close(fd); return nullptr; }
    void *a = mmap(base, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);
    void *b = mmap((uint8_t *)base + 0x10000, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);
    close(fd);
    return a != MAP_FAILED && b != MAP_FAILED ? a : nullptr;
#endif
}

/* ---- host byte order (R1 (b)) ----------------------------------------------------------------
 * A value stored at its own width (a field, a global, through its declared type: xmen2_mem.h
 * mem<T>) is kept in host byte order, and every work-RAM byte has a tag saying how it was stored:
 * 0 a byte (its value as the 68000 sees it), or (width << 4 | index) a byte of a word or long
 * stored whole, index = its place in the unit from the unit's first byte. A read at the width the
 * value was stored (the common case: tags[off] == width << 4) is a host load; anything else (a
 * word read of a long's low half, a byte of a word, two bytes read as a word) is assembled from
 * the bytes as the 68000 sees them, and a store that cuts a unit first turns its remaining bytes
 * into bytes. So the arena always holds exactly the 68000's RAM image, in host order where the
 * code is layout-independent, and the export (xm_ram_image) is a per-unit byte swap. */

}  /* namespace xm */
extern "C" { uint8_t xm_tags[0x10000]; }
namespace xm {

static inline uint8_t be_byte(uint32_t off)
{
    uint8_t t = xm_tags[off];
    if (!t) return xm_arena[off];
    uint32_t w = t >> 4, i = t & 15u, start = off - i;
    return xm_arena[(start + (w - 1u - i)) & 0xFFFFu];
}

/* the unit holding off becomes bytes */
static inline void unit_to_bytes(uint32_t off)
{
    uint8_t t = xm_tags[off];
    if (!t) return;
    uint32_t w = t >> 4, start = off - (t & 15u);
    uint8_t be[4];
    for (uint32_t k = 0; k < w; k++) be[k] = be_byte(start + k);
    for (uint32_t k = 0; k < w; k++) { xm_arena[start + k] = be[k]; xm_tags[start + k] = 0; }
}

static uint64_t s_slow_loads, s_slow_stores;   /* reads / writes not at their value's stored width */

uint32_t mem_load_slow(uint32_t off, int w)
{
    s_slow_loads++;
    if (off + (uint32_t)w > 0x10000u) fatal("a %d-byte read at $FF%04X runs off work RAM", w, (unsigned)off);
    uint32_t v = 0;
    for (int k = 0; k < w; k++) v = v << 8 | be_byte(off + (uint32_t)k);
    return v;
}

void mem_store_slow(uint32_t off, int w, uint32_t v)
{
    s_slow_stores++;
    if (off + (uint32_t)w > 0x10000u) fatal("a %d-byte write at $FF%04X runs off work RAM", w, (unsigned)off);
    unit_to_bytes(off);
    if (w > 1) unit_to_bytes(off + (uint32_t)w - 1u);
    for (int k = 0; k < w; k++) {   /* host order (little-endian), tagged */
        xm_arena[off + (uint32_t)k] = (uint8_t)(v >> (8 * k));
        xm_tags[off + (uint32_t)k] = w > 1 ? (uint8_t)(w << 4 | k) : 0;
    }
}

/* ---- the stack region ------------------------------------------------------------------------ */

static uint32_t s_sp;   /* the mark: a 68000 address in the arena ($00FFxxxx, as the reset SSP) */

uint32_t frame_mark(void) { return s_sp; }
void frame_release(uint32_t mark) { s_sp = mark; }
uint32_t frame_alloc(uint32_t bytes)
{
    s_sp -= (bytes + 1u) & ~1u;
    return s_sp;
}

}  /* namespace xm */

namespace xm { extern uint32_t g_d0; }   /* bridge.py call_native */

extern "C" {
uint8_t  xm_ram_peek8(uint32_t off) { return xm::be_byte(off & 0xFFFFu); }
void     xm_ram_image(uint8_t *out)
{
    for (uint32_t off = 0; off < 0x10000u; off++) out[off] = xm::be_byte(off);
}
uint32_t xm_frame_mark(void) { return xm::frame_mark(); }
void     xm_frame_set(uint32_t mark) { xm::frame_release(mark); }
uint32_t xm_d0_get(void) { return xm::g_d0; }
void     xm_d0_set(uint32_t v) { xm::g_d0 = v; }

/* How the arena holds the game's RAM now: bytes of words and longs stored whole (host order), the
   rest bytes; and how often a read or a write was not at its value's stored width. */
int xm_mem_census(char *buf, int n)
{
    uint32_t units = 0;
    for (uint32_t off = 0; off < 0x10000u; off++) units += xm_tags[off] != 0;
    return snprintf(buf, (size_t)n, "{\"host_order_bytes\": %u, \"byte_bytes\": %u, \"slow_loads\": %llu, "
                    "\"slow_stores\": %llu}", (unsigned)units, (unsigned)(0x10000u - units),
                    (unsigned long long)xm::s_slow_loads, (unsigned long long)xm::s_slow_stores);
}
}

/* Power-on: the arena (zeroed, as the stepped runners' RAM at reset) and the stack region's mark
   at the reset SSP. Before the first game function. */
extern "C" void xm_native_mem_init(void)
{
    const uint16_t one = 1;
    if (*(const uint8_t *)&one != 1) xm::fatal("the native runner keeps game memory in host order: a little-endian host");
    uint8_t *a = (uint8_t *)xm::map_twice();
    if (!a) xm::fatal("cannot map the game-memory arena twice");
    xm_arena = a;
    xm_arena_ffff = a + 0x10000;
    memset(xm_arena, 0, 0x10000);
    xm::s_sp = ((uint32_t)g_rom[0] << 24) | ((uint32_t)g_rom[1] << 16) | ((uint32_t)g_rom[2] << 8) | g_rom[3];
}
