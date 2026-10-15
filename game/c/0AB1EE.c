/* BubbleManager_SpawnAtSurface - BUBBLEMANAGER (surface mode): release one bubble on the surface
 * line g_FloodLineY, 32 to 287 px right of the camera's left edge (g_FrameCounter & $FF), and
 * restart the countdown (period).
 *
 * self: the manager (never runs in the corpus).
 * The bubble grows there first and is held at the line until released (flags 7: grow, held,
 * moving; FloatBubble_Init). */
#include "xmen2.h"

void BubbleManager_SpawnAtSurface(BubbleManager *self)
{
	short flags;
	long x, y;
	FloatBubble *b;

	flags = 7;
	x = (g_CameraX + 0x20 + (g_FrameCounter & 0xFF)) << 8;
	y = g_FloodLineY << 8;
	b = FloatBubble_Spawn(x, y, self->bubbleDir, self->bubbleSpeed, flags);
	self->countdown = self->period;
}
