/* TechWallGun_UpdateBullet - move one wall-gun bullet, end it in a solid map block (or off the
 * map), on a player or 64 px off screen, and draw it.
 *
 * b: the bullet (in use).
 * Flying (flags $80) it moves by its speed, the speed by its acceleration. A live player not
 * blinking whose body box holds the bullet's centre (8 px in) takes 1 damage (TakeDamage, hit
 * type 4, at the bullet) and the bullet ends. During the muzzle flash (flags $40) the frame
 * steps when g_FrameCounter & 3 is 0 (& 2 in two-player step mode); past frame 2 it flies, with
 * the frame and flips of its direction (1-9: TechWallGun_Fire). Drawn as one 16x16 sprite of
 * g_TechWallGunSprDef (palette 3, 4 tiles a frame), with priority when the live player (player
 * 2 when player 1 is not) has it. */
#include "xmen2.h"

void TechWallGun_UpdateBullet(WallGunBullet *b)
{
	short x = b->x >> 8;
	short y = b->y >> 8;
	register XPlayer *p;
	register long attr;
	register long st;
	short i;

	if ((b->flags & 0xF0) & 0x80) {
		b->x += b->vx;
		b->y += b->vy;
		b->vx += b->ax;
		b->vy += b->ay;
	}
	if (x >> 5 >= 0 && x >> 5 < g_FgMapW && y >> 5 >= 0 && y >> 5 < g_FgMapH
	    ? (g_FgBlockMap[g_FgRowTable[y >> 5] + (x >> 5)] & 0x4000 ? 1 : 0) : 1) {
		b->flags = 0;
		return;
	}
	for (i = 0; i < GROUP_N; ++i) {
		if (IS_ALIVE(GROUP_P(i, p)) && !p->hitCooldown
		    && x + 8 > p->boxLeft && x + 8 < p->boxRight && y + 8 > p->boxTop && y + 8 < p->boxBottom) {
			P34_HIT(p, (*p->cls->method[8])(p, 0, 4, 1, x, y));	/* +$26 TakeDamage */
			b->flags = 0;
			return;
		}
	}
	x -= g_CameraX;
	y -= g_CameraY;
	if (x + 64 < 0 || x - 64 > 320 || y + 64 < 0 || y - 64 > 224) {
		b->flags = 0;
		return;
	}
	if ((b->flags & 0xF0) & 0x40) {
		short every = !g_TwoPlayerStep ? 3 : 2;

		if (!(g_FrameCounter & every)) {
			long frame;

			b->frame++;
			frame = b->frame;
			if (frame > 2) {
				b->flags &= ~0x40;
				b->flags |= 0x80;
				switch (b->flags & 0xF) {
				case 1:		/* TechWallGun_UpdateBullet_Case1 */
					b->frame = 4;
					break;
				case 2:
					b->frame = 3;
					break;
				case 3:
					b->flags |= 0x10;
					b->frame = 4;
					break;
				case 4:		/* TechWallGun_UpdateBullet_Case4 */
					b->flags |= 0x20;
					b->frame = 4;
					break;
				case 5:		/* TechWallGun_UpdateBullet_Case5 */
					b->flags |= 0x20;
					b->frame = 3;
					break;
				case 6:
					b->flags |= 0x30;
					b->frame = 4;
					break;
				case 7:
					b->frame = 4;
					break;
				case 8:		/* TechWallGun_UpdateBullet_Case8 */
					b->frame = 5;
					break;
				case 9:		/* TechWallGun_UpdateBullet_Case9 */
					b->flags |= 0x20;
					b->frame = 4;
					break;
				}
			}
		}
	}
	/* TechWallGun_UpdateBullet_Draw */
	attr = g_SpriteDefs[g_TechWallGunSprDef].tileBase | 0x6000;
	st = b->flags & 0xF0;
	attr += b->frame << 2;
	if (st & 0x20)
		attr |= 0x800;
	if (st & 0x10)
		attr |= 0x1000;
	if (PLAYER_LIVE->spr.flags & 4)
		attr |= 0x8000;
	g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
	g_SprWritePtr->y = y + 0x80;
	g_SprWritePtr->size = 5;
	g_SprWritePtr->attr = (short)attr;
	g_SprWritePtr->x = x + 0x80;
}
