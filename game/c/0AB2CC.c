/* BubbleManager_StateEdge - BUBBLEMANAGER state (surface mode): each time the countdown runs out
 * and the surface line g_FloodLineY is on screen (below the camera's top, less than 224 px
 * down), release a bubble on it (BubbleManager_SpawnAtSurface restarts the countdown).
 *
 * self: the manager (never runs in the corpus). */
#include "xmen2.h"

void BubbleManager_StateEdge(BubbleManager *self)
{
	if (!self->countdown && g_FloodLineY > g_CameraY && g_FloodLineY < g_CameraY + 0xE0)
		BubbleManager_SpawnAtSurface(self);
}
