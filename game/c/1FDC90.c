/* VDP_FrameUpdate - the V-int's video hook (g_VBlankHook, Sys_InitGameVideo): mark the tick
 * (g_FrameTick); once per game frame (g_FrameUpdated, cleared by VDP_WaitVBlank) the palette
 * (Pal_Update), the scroll (VDP_UpdateScroll), and when requested (g_DmaFlushReq, cleared) the
 * queued DMAs (VDP_FlushDmaQueue, then g_VBlankFrameDone); every V-int Cyclops' beam flicker
 * (Cyclops_BeamVBlankFlicker). */
#include "xmen2.h"

void VDP_FrameUpdate(void)
{
	g_FrameTick = 1;
	if (!g_FrameUpdated) {
		g_FrameUpdated = 1;
		Pal_Update();
		VDP_UpdateScroll();
		if (g_DmaFlushReq) {
			g_DmaFlushReq = 0;
			VDP_FlushDmaQueue();
			g_VBlankFrameDone = 1;
		}
	}
	Cyclops_BeamVBlankFlicker();
}
