/* Spr_BeginFrame - start this frame's hardware sprite list (Level_FrameLoop).
 *
 * The slot that ended last frame's list (g_SprLast, unless $51: none) links on to the next again;
 * the list is written from entry 81 (g_SprIndex, g_SprWritePtr). */
#include "xmen2.h"

void Spr_BeginFrame(void)
{
	if (g_SprLast != SPR_START)		/* SPR_START: $51 (phase 7: raised, xmen2_mem.h) */
		g_SpriteTable[g_SprLast].link = g_SprLast + 1;
	g_SprIndex = SPR_START;
	g_SprWritePtr = g_SpriteTable + SPR_START;
}
