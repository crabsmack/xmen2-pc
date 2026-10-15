/* SmallBrood_StateExplode - SMALLBROOD kamikaze state: explode at the body box's centre, hurt each
 * live player whose second box touches the blast box (+-$3C x, +-$28 y), then die in a medium
 * burst.
 *
 * self: the object.
 * A hit player gets TakeDamage (class slot +$26) from self with hit type 6 (blast centre left
 * of the player) or 5, self->attack, at the centre point. It is self's hit cooldown that is
 * set to 10 (not the player's), kept. Same blast as DropBomb_StateExplode. */
#include "xmen2.h"

void SmallBrood_StateExplode(SmallBrood *self)
{
	long cx = (self->boxRight + self->boxLeft) << 7;
	long cy = (self->boxTop + self->boxBottom) << 7;
	short px = (short)(cx >> 8);
	short py = (short)(cy >> 8);
	short left = px - 0x3C;
	short top = py - 0x28;
	short right = left + 0x78;
	short bottom = top + 0x50;
	long i;
	XPlayer *p;

	Explosion_Spawn(cx, cy, 0);
	for (i = 0; i < 2; ++i) {
		if (IS_ALIVE(i == 0 ? (p = &g_Player1) : (p = &g_Player2)) && p->hitCooldown == 0
		    && left <= p->box2Right && right >= p->box2Left && top <= p->box2Bottom
		    && bottom >= p->box2Top) {
			short dir = cx < p->x ? 6 : 5;

			(*p->cls->method[8])(p, self, dir, self->attack, px, py);	/* +$26 TakeDamage */
			self->hitCooldown = 10;
		}
	}
	ExplodeFx_BurstMedium(self);
}
