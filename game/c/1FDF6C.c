/* VDP_ClearVram - clear all 64 KB of VRAM with a DMA fill of zero, and wait for it.
 *
 * Auto-increment 1, DMA on (register 1 shadow g_VdpReg1 | $10), length $FFFF, fill mode,
 * VRAM write at 0, fill byte 0; polls status bit 1 (DMA busy), then restores register 1 and
 * auto-increment 2. Hand-written 68000; Sys_InitHardware and Level_Load call it. */
#include "xmen2.h"

void VDP_ClearVram(void)
{
	VDP_CTRL = 0x8F01;
	VDP_CTRL = g_VdpReg1 | 0x10;
	VDP_CTRL = 0x93FF;
	VDP_CTRL = 0x94FF;
	VDP_CTRL = 0x9780;
	VDP_CTRL = 0x4000;
	VDP_CTRL = 0x0080;
	VDP_DATA_B = 0;
	while (VDP_CTRL & 2)
		;
	VDP_CTRL = g_VdpReg1;
	VDP_CTRL = 0x8F02;
}
