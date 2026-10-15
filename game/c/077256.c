/* CloneA1_AnimTilesTimer - CloneA1's V-blank hook every 60 frames: show the other frame of two
 * animated tile sets, and clear the other half with a DMA fill.
 *
 * Even V-blank counts: tiles from Art_CloneA1Nem_2 + $410 to the second half of the animated
 * area (g_AnimTiles * 32 + $380, $E0 bytes) and Tbl_CloneA1Vdp + $CC to g_FgTilesEnd; then
 * fill the first half ($380 bytes) with 0. Odd counts: + $90 to the first half, + $6CC, and
 * fill the second half. The fill is a VDP DMA fill with auto-increment 1 (restored to 2).
 * Hand-written 68000; installed by CloneA1_RunScene (Timer_Create(Timer_SetVBlankHookCb, $3C,
 * CloneA1_AnimTilesTimer, 1)). */
#include "xmen2.h"

#define g_AnimTiles (*(s16 *)g_SceneVar3B10)	/* CloneA1: first VRAM tile of the animated area */

void CloneA1_AnimTilesTimer(void)
{
	u8 *art = (u8 *)CODEPTR(Art_CloneA1Nem_2);
	u8 *vdp = (u8 *)CODEPTR(Tbl_CloneA1Vdp);
	unsigned short dest;
	unsigned short len;

	if (!(g_VBlankCount & 1)) {
		VDP_DmaToVram(ADDR68(art + 0x410), (g_AnimTiles << 5) + 0x380, 0xE0);
		VDP_DmaToVram(ADDR68(vdp + 0xCC), g_FgTilesEnd << 5, 0x600);
		dest = g_AnimTiles << 5;
		len = 0x380;
	} else {
		VDP_DmaToVram(ADDR68(art + 0x90), g_AnimTiles << 5, 0x380);
		VDP_DmaToVram(ADDR68(vdp + 0x6CC), g_FgTilesEnd << 5, 0x600);
		dest = (g_AnimTiles << 5) + 0x380;
		len = 0xE0;
	}
	len--;
	VDP_CTRL = g_VdpReg1 | 0x10;
	VDP_CTRL = 0x8F01;
	VDP_CTRL = 0x9300 | (len & 0xFF);
	VDP_CTRL = 0x9400 | len >> 8;
	VDP_CTRL = 0x9780;
	VDP_CTRL = 0x4000 | (dest & 0x3FFF);
	VDP_CTRL = 0x0080 | dest >> 14;
	VDP_DATA_B = 0;
	while (VDP_CTRL & 2)
		;
	VDP_CTRL = g_VdpReg1;
	VDP_CTRL = 0x8F02;
}
