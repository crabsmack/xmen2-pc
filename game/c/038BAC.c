/* BaniMazaA1_InitScroll - the Moving Blocks' scrolling: the rolling-disc bands are the level's
 * scroll update (g_LevelScrollHook = RollingDiscs_UpdateScroll), the free H-scroll area from
 * tile $7BE ($42 tiles), H-scroll per line (VDP register 11 = g_VdpReg11 + 3), and the H-scroll
 * buffer cleared ($1C0 words: plane A and B of 224 lines). */
#include "xmen2.h"

void BaniMazaA1_InitScroll(void)
{
	register long i;

	g_LevelScrollHook = FN_DATA(RollingDiscs_UpdateScroll);
	g_HScrollFreeTile = 0x7BE;
	g_HScrollFreeCount = 0x42;
	VDP_CTRL = g_VdpReg11 + 3;
	for (i = 0; i < 0x1C0; i++)
		((s16 *)g_HScrollBuf)[i] = 0;
}
