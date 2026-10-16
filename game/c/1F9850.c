/* Sys_InitGameVideo - game start-up (Game_Main): Game_Init, then the palette and DMA state reset,
 * VDP register $10 = 1 (64x32 planes), the base palette cleared, VDP_FrameUpdate as the V-int hook
 * and the button masks (Game_InitButtonMasks). */
#include "xmen2.h"

void Sys_InitGameVideo(void)
{
	register short i;

	Game_Init();
	g_PalBaseDirty = 0;
	g_PalFadeActive = 0;
	g_DmaFlushReq = 0;
	g_PalFadeDelay = 4;
	g_BgLockToObj = 0;
	VDP_CTRL = 0x9001;
	for (i = 0; i < 64; i++)
		g_PalBase[i] = 0;
	g_VBlankHook = VDP_FrameUpdate;
	Game_InitButtonMasks();
}
