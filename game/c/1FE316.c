/* VDP_DmaPalette - DMA a 64-colour palette (128 bytes) to CRAM, then rewrite its first word.
 *
 * pal: 64 colour words (g_PalWork or g_PalBase: Pal_Update).
 * Length $40 words, command $C000/$0080 kept in g_VdpCmdHi/Lo (CRAM write at 0, DMA); after
 * the DMA the command goes again without its DMA bit and colour 0 is written by hand (the
 * VDP's first DMA word to CRAM can be lost). No MPW frame, but compiled-C shapes (asm tag). */
#include "xmen2.h"

void VDP_DmaPalette(s16 *pal)
{
	unsigned long src = ADDR68(pal);

	g_VdpCmdHi = (short)0xC000;
	g_VdpCmdLo = 0x80;
	VDP_CTRL = g_VdpReg1 | 0x10;
	VDP_CTRL = 0x9340;
	VDP_CTRL = 0x9400;
	VDP_CTRL = 0x9500 + (src >> 1 & 0xFF);
	VDP_CTRL = 0x9600 + (src >> 9 & 0xFF);
	VDP_CTRL = 0x9700 + (src >> 17 & 0x7F);
	VDP_CTRL = g_VdpCmdHi;
	VDP_CTRL = g_VdpCmdLo;
	while (VDP_CTRL & 2)
		;
	g_VdpCmdLo &= 0xFF7F;
	VDP_CTRL = g_VdpCmdHi;
	VDP_CTRL = g_VdpCmdLo;
	VDP_DATA = *pal;
	VDP_CTRL = g_VdpReg1;
}
