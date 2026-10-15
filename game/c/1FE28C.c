/* VDP_FlushDmaQueue - V-blank DMA: the sprite table to VRAM $F000, then every queued DMA.
 *
 * With the sound driver paused (Z80_DriverPause/Resume): DMA $140 words of g_SpriteTable to
 * VRAM $F000 (command $7000/$0083), wait; then for each DmaEntry write its eight register
 * words to the control port and wait for the DMA; empty the queue. Hand-written 68000;
 * VDP_FrameUpdate calls it when g_DmaFlushReq is set. */
#include "xmen2.h"

void VDP_FlushDmaQueue(void)
{
	unsigned long s;
	DmaEntry *e;
	unsigned short k;

	Z80_DriverPause();
	VDP_CTRL = g_VdpReg1 | 0x10;
	VDP_CTRL = 0x9340;
	VDP_CTRL = 0x9401;
	s = ADDR68(g_SpriteTable) >> 1;
	VDP_CTRL = 0x9500 | (s & 0xFF);
	VDP_CTRL = 0x9600 | (s >> 8 & 0xFF);
	VDP_CTRL = 0x9700 | (s >> 16 & 0x7F);
	VDP_CTRL = 0x7000;
	VDP_CTRL = 0x0083;
	while (VDP_CTRL & 2)
		;
	VDP_CTRL = g_VdpReg1;
	e = g_DmaQueue;
	for (k = g_DmaQueueCount; k; k--) {
		VDP_CTRL = e->regMode;
		VDP_CTRL = e->regLenLo;
		VDP_CTRL = e->regLenHi;
		VDP_CTRL = e->regSrcLo;
		VDP_CTRL = e->regSrcMid;
		VDP_CTRL = e->regSrcHi;
		VDP_CTRL = (unsigned short)(e->cmd >> 16);
		VDP_CTRL = (unsigned short)e->cmd;
		while (VDP_CTRL & 2)
			;
		VDP_CTRL = g_VdpReg1;
		e++;
	}
	g_DmaQueueCount = 0;
	Z80_DriverResume();
}
