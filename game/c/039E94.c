/* BanimazaDoor_DrawDebris - draw the shards of a shattered door and move them: each is one 16x16
 * hardware sprite (size 5) of tile tileBase + $30 + 4 * an animation frame 0-7 that steps every
 * 8 frames (offset per shard), when on screen; then its velocity moves it and its acceleration
 * changes the velocity.
 *
 * s: the shards.  n: how many (at least 1).  tileBase: the door's first VRAM tile.
 * The ROM saves d4 for a variable it never uses (frame below: the dead store reproduces it). */
#include "xmen2.h"

void BanimazaDoor_DrawDebris(register DoorShard *s, short n, short tileBase)
{
	register long frame;
	register long sx;
	register long sy;
	register long tile;

	tileBase += 0x30;
	do {
		sx = (s->x >> 8) - g_CameraX;
		sy = (s->y >> 8) - g_CameraY;
		if (0)
			frame = 0;
		tile = ((n + g_FrameCounter) >> 3 & 7) << 2;
		if (sx > VIEW_L(-32) && sx < VIEW_R(320) && sy > -32 && sy < 224) {
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = sy + 0x80;
			g_SprWritePtr->size = 5;
			g_SprWritePtr->attr = tileBase + tile;
			g_SprWritePtr->x = sx + 0x80;
		}
		s->x += s->vx;
		s->y += s->vy;
		s->vx += s->ax;
		s->vy += s->ay;
		s++;
		--n;
	} while (n);
}
