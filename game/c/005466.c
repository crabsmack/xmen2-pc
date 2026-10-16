/* Game_Main - the main program (from the boot code): RAM above the stack cleared, sound on, then
 * once the VDP is out of its DMA (status bit 1) the button masks of the 7 actions
 * (Pad_ConfigButtonMask), the hardware, the sprite links and the game's video; then scenes
 * (Game_SceneLoop) forever, each pass out of demo mode. Never returns. */
#include "xmen2.h"

void Game_Main(void)
{
	register short i;

	Mem_ClearAboveStack();
	g_ExtIntMode[0] = 0;
	g_SndMute = 0;
	while (VDP_CTRL & 2)
		;
	for (i = 0; i <= 6; i++)
		g_ActionButtonMasks[i] = Pad_ConfigButtonMask(i);
	Sys_InitHardware();
	Spr_InitLinks();
	g_InLevelLoop = 0;
	g_FrameTick = 0;
	g_FrameUpdated = 1;
	Sys_InitGameVideo();
	for (;;) {
		g_DemoActive = 0;
		Game_SceneLoop();
	}
}
