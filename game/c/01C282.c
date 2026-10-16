/* YoyoAcolyte_Draw - YOYOACOLYTE Draw (class slot +$1A): its yoyo, then itself.
 *
 * self: the acolyte.
 * While the yoyo is out and the acolyte alive: the yoyo moves one step (offset += speed,
 * speed += acceleration) and spins (frames 0-3, 2 frames each); it is drawn where it was before
 * the step, as one 32x32 hardware sprite of the g_YoyoAcolyteSprDef2 tiles (palette 1, h-flipped
 * with the acolyte), when on screen. Each live player not in its hit cooldown whose second box
 * touches the yoyo's 26 px box (3 px in from the sprite's corner) gets TakeDamage (class slot
 * +$26) from self, hit type 10 when the yoyo is left of the player (else 9), self->attack, at the
 * box centre; it is self's hit cooldown that is set to 10 (not the player's), kept. The
 * test `flags & ~2` (meant: not caught) is always true with bit 0 set, kept. Then the parent's
 * (BIONPC) Draw. */
#include "xmen2.h"

void YoyoAcolyte_Draw(YoyoAcolyte *self)
{
	short x = (self->x >> 8) - g_CameraX;
	short y = (self->y >> 8) - g_CameraY;
	short flags = self->yoyoFlags;

	if (flags & 1 && flags & ~2 && self->health > 0) {
		register YoyoAcolyteYoyo *yo = &self->yoyo;
		short sx = x + (yo->x >> 8);
		short sy = y + (yo->y >> 8);
		short attr;

		yo->x += yo->vx;
		yo->y += yo->vy;
		yo->vx += yo->ax;
		yo->vy += yo->ay;
		--yo->frameTimer;
		if (yo->frameTimer <= 0) {
			yo->frameTimer = 2;
			yo->frame++;
			if (yo->frame > 3)
				yo->frame = 0;
		}
		attr = g_SpriteDefs[g_YoyoAcolyteSprDef2].tileBase | 0x2000;
		attr += yo->frame << 4;
		if (self->spr.flags & 0x10)
			attr |= 0x800;
		if (sx > -32 && sx < 320 && sy > -32 && sy < 224) {
			g_SprWritePtr = g_SpriteTable + (g_SprIndex = g_SprWritePtr->link);
			g_SprWritePtr->y = sy + 0x80;
			g_SprWritePtr->size = 0xF;
			g_SprWritePtr->attr = attr;
			g_SprWritePtr->x = sx + 0x80;
		}
		{
			short left = g_CameraX + sx + 3;
			short top = g_CameraY + sy + 3;
			short right = left + 26;
			short bottom = top + 26;
			short cx = left + right >> 1;
			short cy = top + bottom >> 1;
			register XPlayer *p;

			p = &g_Player1;
			if (IS_ALIVE(p) && !p->hitCooldown && left <= p->box2Right && right >= p->box2Left
			    && top <= p->box2Bottom && bottom >= p->box2Top) {
				short px = p->x >> 8;
				short type = cx < px ? 10 : 9;

				(*p->cls->method[8])(p, self, type, self->attack, cx, cy);	/* +$26 TakeDamage */
				self->hitCooldown = 10;
			}
			p = &g_Player2;
			if (IS_ALIVE(p) && !p->hitCooldown && left <= p->box2Right && right >= p->box2Left
			    && top <= p->box2Bottom && bottom >= p->box2Top) {
				short px = p->x >> 8;
				short type = cx < px ? 10 : 9;

				(*p->cls->method[8])(p, self, type, self->attack, cx, cy);	/* +$26 TakeDamage */
				self->hitCooldown = 10;
			}
		}
	}
	(*CLASS(Tbl_YoyoAcolyte_vtable)->parent->method[5])(self);	/* BIONPC Draw */
}
