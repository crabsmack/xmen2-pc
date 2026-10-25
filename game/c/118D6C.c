/* GaussBullet_Update - one gauss bullet's frame (GaussBullet_UpdateAll): move it, remove it when
 * it hits a player, a solid cell or leaves the screen, else step its animation and draw it.
 *
 * b:     the record (in g_GaussPool).
 * steps: frames this update stands for (2 in two-player stepping).
 * A bullet following its owner (flags bit 2) is placed at the owner's position + its offset; one
 * animating in place (bit 1) doesn't move; any other moves steps times by its velocity and
 * acceleration. It is removed (the pool's last record copied over it) outside the FG map, on a
 * cell with block flag $4000, more than 20 px off the screen (244 below the top), or when it
 * reaches a live player not flashing from a hit, whose second box holds it: the player takes a hit
 * (TakeDamage: no attacker, hit type 4, 1 damage). The in-place animation steps a frame every 4
 * frames and ends at frame 4 or 9. The sprite is the GaussFrame's size and tile (from g_GaussSprDef's first tile) with the
 * bullet's attribute bits. (After a removal the caller's loop skips the record copied in.) */
#include "xmen2.h"

#define REMOVE(b) \
	if (g_GaussPool->count) \
		Mem_Copy(b, &g_GaussPool->rec[--g_GaussPool->count], sizeof(GaussBullet))

void GaussBullet_Update(GaussBullet *b, short steps)
{
	short n = steps;
	register long i;
	register XPlayer *p;
	GaussFrame *f;
	short left, top, right, bottom;
	short minX, minY, maxX;

	if (b->flags & 4) {
		long ox = b->owner->x;
		long oy = b->owner->y;

		b->x = (b->offX << 8) + ox;
		b->y = (b->offY << 8) + oy;
	} else if (!(b->flags & 2)) {
		do {
			b->x += b->vx;
			b->y += b->vy;
			b->vx += b->ax;
			b->vy += b->ay;
		} while (--n);
	}
	{
		short x = b->x >> 8;
		short y = b->y >> 8;
		register short maxY;

		if ((x >> 5 >= 0 && x >> 5 < g_FgMapW && y >> 5 >= 0 && y >> 5 < g_FgMapH)
		    ? (g_FgBlockMap[g_FgRowTable[y >> 5] + (x >> 5)] & 0x4000 ? 1 : 0) : 1) {
			REMOVE(b);
			return;
		}
		for (i = 0; i < GROUP_N; ++i) {
			if (IS_ALIVE(GROUP_P(i, p))) {
				left = p->box2Left;
				top = p->box2Top;
				right = p->box2Right;
				bottom = p->box2Bottom;
				if (!p->hitCooldown && x >= left && x <= right && y >= top && y <= bottom) {
					P34_HIT(p, (*p->cls->method[8])(p, 0, 4, 1, x, y));	/* +$26 TakeDamage */
					REMOVE(b);
					return;
				}
			}
		}
		x -= g_CameraX;
		y -= g_CameraY;
		minX = -20;
		minY = -20;
		maxX = 340;
		maxY = 244;
		if (x < minX || x > maxX || y < minY || y > maxY) {
			REMOVE(b);
			return;
		}
		if (b->flags & 2) {
			n = steps;
			do {
				if (!--b->delay) {
					b->delay = 4;
					b->frame++;
					if (b->frame == 4 || b->frame == 9) {
						b->flags &= ~6;
						break;
					}
				}
			} while (--n);
		}
		f = g_GaussPool->frames;
		f += b->frame;
		{
			char size = f->size;
			register short attr = g_SpriteDefs[g_GaussSprDef].tileBase + f->tile;

			attr |= b->flags & 0xF800;
			x -= f->dx;
			y -= f->dy;
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = y + 0x80;
			g_SprWritePtr->size = size;
			g_SprWritePtr->attr = attr;
			g_SprWritePtr->x = x + 0x80;
		}
	}
}
