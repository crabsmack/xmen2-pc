/* VDP_DmaToVram - DMA a block from 68000 memory to VRAM now, and wait for it to finish.
 *
 * src: 68000 source address.  dest: VRAM address (low word).  len: length (low word; the
 * register gets len / 2 words).
 * DMA on through register 1's shadow g_VdpReg1 | $10, length and source registers $93-$97,
 * command $4000|dest / $80|dest>>14 (VRAM write, DMA); polls status bit 1, then restores
 * register 1. Hand-written 68000; VDP_QueueDma calls it while the display is off. */
#include "xmen2.h"

void VDP_DmaToVram(long src, long dest, long len)
{
	unsigned short d = (unsigned short)dest;
	unsigned short words = (unsigned short)len >> 1;
	unsigned long s = (unsigned long)src >> 1;

	VDP_CTRL = g_VdpReg1 | 0x10;
	VDP_CTRL = 0x9300 | (words & 0xFF);
	VDP_CTRL = 0x9400 | words >> 8;
	VDP_CTRL = 0x9500 | (s & 0xFF);
	VDP_CTRL = 0x9600 | (s >> 8 & 0xFF);
	VDP_CTRL = 0x9700 | (s >> 16 & 0x7F);
	VDP_CTRL = 0x4000 | (d & 0x3FFF);
	VDP_CTRL = 0x0080 | d >> 14;
	while (VDP_CTRL & 2)
		;
	VDP_CTRL = g_VdpReg1;
}
