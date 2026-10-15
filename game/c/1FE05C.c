/* unk_VDP_1FE05C - at init, DMA the 4 bytes of g_VBlankCount to VRAM $ED00 with the display off.
 *
 * Register 1 $8114 (display off, DMA on), length 2 words, source g_VBlankCount (the V-int frame
 * counter its caller just zeroed), command $6D00/$0083 kept in g_VdpCmdHi/Lo (VRAM $ED00, DMA),
 * then register 1 $8104. Only Sys_InitHardware calls it; what VRAM $ED00 is for is not known.
 * No MPW frame, but the shape of compiled C (asm tag: it can't be byte-matched). */
#include "xmen2.h"

void unk_VDP_1FE05C(void)
{
	unsigned long src;

	g_VdpCmdHi = 0x6D00;
	g_VdpCmdLo = 0x83;
	src = ADDR68(&g_VBlankCount);
	VDP_CTRL = 0x8114;
	VDP_CTRL = 0x9302;
	VDP_CTRL = 0x9400;
	VDP_CTRL = 0x9500 + (src >> 1 & 0xFF);
	VDP_CTRL = 0x9600 + (src >> 9 & 0xFF);
	VDP_CTRL = 0x9700 + (src >> 17 & 0x7F);
	VDP_CTRL = g_VdpCmdHi;
	VDP_CTRL = g_VdpCmdLo;
	VDP_CTRL = 0x8104;
}
