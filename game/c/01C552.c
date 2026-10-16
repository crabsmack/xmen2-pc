/* YoyoAcolyte_ThrowYoyo - YOYOACOLYTE class slot +$82 (its own): throw the yoyo.
 *
 * self: the acolyte.  kind: 0 level (also anything but 1, 2), 1 high arc, 2 low arc.
 * No animation, frame 9; hittable again (its body node back in g_HitListEnemyBody, flags6 bit 1
 * off); yoyo out. The yoyo starts 64 px up and 40 px ahead of x facing right (72 px behind it
 * facing left: the sprite's corner), at speed $800 forward, decelerating by $46 per frame; high:
 * vertical speed -$300, gravity $1A; low: $200, -$11; level: none. */
#include "xmen2.h"

void YoyoAcolyte_ThrowYoyo(register YoyoAcolyte *self, short kind)
{
	register YoyoAcolyteYoyo *yo = &self->yoyo;
	register char left = self->spr.flags & 0x10;

	self->anim = 0;
	(*self->cls->method[20])(self, 9);		/* +$56 SetFrame */
	self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	self->flags6 &= ~2;
	self->yoyoFlags = 1;
	yo->frame = 0;
	yo->frameTimer = 2;
	if (left) {
		yo->x = -0x4800;
		yo->vx = -0x800;
		yo->ax = 0x46;
	} else {
		yo->x = 0x2800;
		yo->vx = 0x800;
		yo->ax = -0x46;
	}
	yo->y = -0x4000;
	switch (kind) {
	default:
	case 0:
		yo->vy = 0;
		yo->ay = 0;
		break;
	case 1:
		yo->vy = -0x300;
		yo->ay = 0x1A;
		break;
	case 2:
		yo->vy = 0x200;
		yo->ay = -0x11;
	}
}
