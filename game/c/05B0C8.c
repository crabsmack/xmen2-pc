/* Boulder_UpdateDebris - move and draw the live rock-debris pieces (twice the motion in two-player
 * stepping): velocity, then drag and gravity; a piece below its floor line (screen y) is put
 * back on it and bounces once (vy = (rand & $FF) + $190 - vy for a big piece, + $C8 for a small
 * one, upwards), and its drag stops with its x speed once both have one sign. A piece off the
 * screen (x > $154 or < -20, y > $F0) is freed; the others are drawn as one hardware sprite,
 * high priority, palette 2, from the tiles of g_BoulderDebrisSprDef: big 16x16 (size 5, tiles 8 +
 * 4 * (counter & 3)), small 8x8 (tile counter & 7), the counter stepping each frame.
 *
 * A freed piece's slot gets the last live piece, which this pass then skips (the loop moves on). */
#include "xmen2.h"

void Boulder_UpdateDebris(void)
{
	register BoulderDebris *d;
	register SprInst *def;
	register short i;
	register short sx, sy;
	register short attr;

	d = g_BoulderDebrisPool;
	for (i = 0; i < g_BoulderDebrisCount; i++, d++) {
		d->x += d->vx;
		d->y += d->vy;
		d->vx += d->ax;
		d->vy += d->ay;
		if (g_TwoPlayerStep) {
			d->x += d->vx;
			d->y += d->vy;
			d->vx += d->ax;
			d->vy += d->ay;
		}
		sx = (d->x >> 8) - g_CameraX;
		sy = (d->y >> 8) - g_CameraY;
		if (sy > d->floorY) {
			d->y = (d->floorY + g_CameraY << 8) - d->vy;
			if (d->big)
				d->vy = -(d->vy - (((unsigned short)Rand_Next() & 0xFF) + 0x190));
			else
				d->vy = -(d->vy - (((unsigned short)Rand_Next() & 0xFF) + 0xC8));
			d->floorY = 0x7D00;
		}
		if (d->ax < 0 && d->vx < 0 || d->ax > 0 && d->vx > 0) {
			d->vx = 0;
			d->ax = 0;
		}
		if (sx > 0x154 || sx < -20 || sy > 0xF0)
			Boulder_FreeDebris(d);
		else {
			def = &g_SpriteDefs[g_BoulderDebrisSprDef];
			if (d->big) {
				attr = ((d->frame++ & 3) << 2) + 8;
				attr = def->tileBase + attr | 0x4000 | 0x8000;
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = sy + 0x80;
				g_SprWritePtr->size = 5;
				g_SprWritePtr->attr = attr;
				g_SprWritePtr->x = sx + 0x80;
			} else {
				attr = def->tileBase + (d->frame++ & 7) | 0x4000 | 0x8000;
				g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
				g_SprWritePtr->y = sy + 0x80;
				g_SprWritePtr->size = 0;
				g_SprWritePtr->attr = attr;
				g_SprWritePtr->x = sx + 0x80;
			}
		}
	}
}
