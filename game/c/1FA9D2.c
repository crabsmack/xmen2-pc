/* Spr_EndFrame - end this frame's hardware sprite list (Level_FrameLoop, before the DMA).
 *
 * The last slot written (g_SprIndex; 79 when the list ran to 80) gets link 0 and is remembered
 * in g_SprLast for Spr_BeginFrame. */
#include "xmen2.h"

void Spr_EndFrame(void)
{
	g_SprIndex = g_SprIndex == SPR_STOP ? SPR_STOP - 1 : g_SprIndex;	/* 80 (phase 7: raised, xmen2_mem.h) */
	g_SpriteTable[g_SprIndex].link = 0;
	g_SprLast = g_SprIndex;
}
