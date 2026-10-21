/* game_entry.cpp - the native runner's two ways into the game (phase 5 P6): the reset entry and the
 * level-6 interrupt vector, by their C names (the bridge's wrappers), as the ROM's vector table
 * names them ($000004 -> EntryPoint, $000078 -> Irq_VBlankVector; checked at start). */
#include "native.h"
#include "xmen2.h"

static uint32_t rom32(uint32_t a)
{
    return (uint32_t)g_rom[a] << 24 | (uint32_t)g_rom[a + 1] << 16 | (uint32_t)g_rom[a + 2] << 8 | g_rom[a + 3];
}

extern "C" void xm_entry_point(void)
{
    if (rom32(4) != 0x1F43AEu || rom32(0x78) != 0x1F4860u)
        xm::fatal("the ROM's reset / V-int vectors ($%06X / $%06X) are not EntryPoint / Irq_VBlankVector",
                  (unsigned)rom32(4), (unsigned)rom32(0x78));
    EntryPoint();
}

extern "C" void xm_vint_handler(void) { Irq_VBlankVector(); }
