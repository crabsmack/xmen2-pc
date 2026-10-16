/* Cerebro_ClearScreen - fill the 40x32 visible part of the 64-column plane at VRAM $0000 with the
 * blank tile $86C0. */
#include "xmen2.h"

/* The VDP control word for a VRAM write at byte address a, as the original summed it. */
#define VRAM_WRITE(a)	((((a) & 0x3FFF) << 16) + 0x40000000 + ((unsigned long)((a) & 0xC000) >> 14))

void Cerebro_ClearScreen(void)
{
	register short row;
	register short col;

	for (row = 0; row <= 31; ++row) {
		VDP_CTRL32 = VRAM_WRITE(row << 7);
		for (col = 0; col <= 39; ++col)
			VDP_DATA = 0x86C0;
	}
}
