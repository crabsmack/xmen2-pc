/* Debris_Update - a debris pool's timer (every frame): move its pieces, bounce them on the
 * floor line, drop the worn-out ones and draw the rest as 16x16 hardware sprites.
 *
 * pool: the pool (the timer's argument). Nothing happens while it has no sprite slot (-1).
 * A piece below the floor is mirrored back above it, keeps 1.5 times its vx and bounces with
 * -vy / 4 (bounce + 3); it is gone (the last piece copied over it, the loop runs that slot again)
 * when its bounce passes the pool's limit or |vy| is under $40. Its frame (0-2) steps every
 * (6 - bounce) / 2 + 1 frames; tiles from the slot's, 4 per (bounce + frame), the slot's palette,
 * high priority, h-flip from spin bit 1. Only pieces near the screen (x -19..339, y -9..253 from
 * 12 px above the camera's top) are drawn. */
#include "xmen2.h"

void Debris_Update(register DebrisPool *pool)
{
	short attr;			/* the sprites' attribute word: priority, palette, first tile */
	short tile;			/* one piece's */
	SprInst *def;
	DebrisPiece *pieces = (DebrisPiece *)&pool[1];
	register DebrisPiece *p = pieces;
	register short i;
	register char bounced;
	register short x, y;

	if (pool->sprDef < 0)
		return;
	def = &g_SpriteDefs[pool->sprDef];
	attr = def->tileBase | (def->flags & 3) << 13 | 0x8000;
	for (i = 0; i < pool->used; i++, p++) {
		bounced = 0;
		p->x += p->vx;
		p->y += p->vy;
		p->vy += p->gravity;
		x = p->x >> 8;
		y = p->y >> 8;
		if (y > pool->floorY) {
			p->y = (pool->floorY - (y - pool->floorY)) << 8;
			y = p->y >> 8;
			p->vx += p->vx >> 1;
			bounced = 1;
		}
		if (bounced) {
			p->bounce += 3;
			p->vy = -(p->vy >> 2);
			if (p->bounce > pool->bounceLimit || ABS(p->vy) < 0x40) {
				if (pool->used)
					Mem_Copy(p, &pieces[--pool->used], sizeof(DebrisPiece));
				i--;
				p--;
				continue;
			}
		}
		if (p->frameDelay-- <= 0) {
			p->frame++;
			if (p->frame > 2)
				p->frame = 0;
			p->frameDelay = ((6 - p->bounce) >> 1) + 1;
		}
		p->spin++;
		x -= g_CameraX - 0;		/* on screen, 12 px lower */
		y -= g_CameraY - -0xC;
		if (x > -0x14 && x < 0x154 && y > -0xA && y < 0xFE) {
			tile = attr + ((p->bounce + p->frame) << 2);
			tile |= (p->spin & 2) << 10;
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = y + 0x80;
			g_SprWritePtr->size = 5;
			g_SprWritePtr->attr = tile;
			g_SprWritePtr->x = x + 0x80;
		}
	}
}
