/* game_entry.cpp - the native runner's two ways into the game (phase 5 P6): the reset entry and the
 * level-6 interrupt vector, by their C names (the bridge's wrappers), as the ROM's vector table
 * names them ($000004 -> EntryPoint, $000078 -> Irq_VBlankVector; checked at start). */
#include <stdio.h>
#include <stdlib.h>
#include "native.h"
#include "xmen2.h"

static uint32_t rom32(uint32_t a)
{
    return (uint32_t)g_rom[a] << 24 | (uint32_t)g_rom[a + 1] << 16 | (uint32_t)g_rom[a + 2] << 8 | g_rom[a + 3];
}

/* XMEN2_NULL_SELFTEST=1: a game type read through a null pointer must read what the 68000 reads
   there, the vector table (xmen2_mem.h mem<T>; P5-port.md "Platform layer": in the memory layer,
   not in scattered guards). Exits 0 when the word at +4 and the long at +$78 are the ROM's. */
struct NullProbe { u8 pad[4]; s16 w; u8 pad2[0x72]; u32 l; };
static void null_selftest(void)
{
    NullProbe *volatile p = nullptr;
    unsigned w = (uint16_t)p->w, l = (uint32_t)p->l;
    unsigned want_w = (unsigned)(g_rom[4] << 8 | g_rom[5]), want_l = (unsigned)rom32(0x78);
    fprintf(stderr, "[native] null-read self-test: +4 $%04X (ROM $%04X), +$78 $%08X (ROM $%08X): %s\n",
            w, want_w, l, want_l, w == want_w && l == want_l ? "ok" : "FAILED");
    exit(w == want_w && l == want_l ? 0 : 1);
}

extern "C" void xm_entry_point(void)
{
    if (const char *e = getenv("XMEN2_NULL_SELFTEST"); e && *e == '1') null_selftest();
    if (rom32(4) != 0x1F43AEu || rom32(0x78) != 0x1F4860u)
        xm::fatal("the ROM's reset / V-int vectors ($%06X / $%06X) are not EntryPoint / Irq_VBlankVector",
                  (unsigned)rom32(4), (unsigned)rom32(0x78));
    EntryPoint();
}

extern "C" void xm_vint_handler(void) { Irq_VBlankVector(); }
