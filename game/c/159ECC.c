/* Cam_ClampLookToPlayers - fold the camera's look offset into its y, as far as keeps every live
 * player inside the dead zone's rows.
 *
 * cam: the camera (Nightcrawler's teleport calls it each frame).
 * With a look offset: camFlags $10 (the look steps pause), y += look, then y is clamped so the
 * highest live player stays at or below deadTop and the lowest at or above deadBottom; what the
 * clamp takes off stays in look. The position and look are read and written through g_Camera
 * (cam only gets the flag): the original's.
 * The clamps are comma expressions (a temporary read in one expression gets no copy of its own),
 * the first through a stack variable, the second through maxCamY's register: the ROM's. */
#include "xmen2.h"

void Cam_ClampLookToPlayers(Camera *cam)
{
	register Camera *c = cam;
	long topY = 9999999;
	long bottomY = -9999999;
	short y = g_Camera->y;
	short look = g_Camera->look;
	short to;
	short topPix, botPix;
	short minCamY, maxCamY;

	if (look) {
		c->camFlags |= 0x10;
		if (IS_ALIVE(&g_Player1)) {
			topY = g_Player1.y;
			bottomY = g_Player1.y;
		}
		if (IS_ALIVE(&g_Player2)) {
			topY = MIN(topY, g_Player2.y);
			bottomY = MAX(bottomY, g_Player2.y);
		}
		topPix = topY >> 8;
		botPix = bottomY >> 8;
		maxCamY = topPix - c->deadTop;
		minCamY = botPix - c->deadBottom;
		y += look;
		look = 0;
		if (y > maxCamY)
			to = maxCamY, look = y - to, y = to;
		else if (y < minCamY)
			maxCamY = minCamY, look = y - maxCamY, y = maxCamY;
		g_Camera->y = y;
		g_Camera->look = look;
	}
}
