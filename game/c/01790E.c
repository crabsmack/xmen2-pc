/* FarAcolyte_MoveOrb - FARACOLYTE slot +$72: move the orb 1/16 of the way to the centre of the
 * nearest player's body box, keeping it inside rangeLeft..rangeRight.
 *
 * self: the acolyte (FarAcolyte_StateOrbChase).
 * The orb's vx ends as the low word of the x step it really made (after the clamp).
 * Returns 1 when the orb is within 16 px (|dx| + |dy|, dy before the move) of the target. */
#include "xmen2.h"

char FarAcolyte_MoveOrb(register FarAcolyte *self)
{
	XPlayer *p;
	long tx, ty;
	register FarAcolyteShot *orb;
	long ox, oy;
	register long dx, dy;

	p = Player_Nearest((Actor *)self);
	orb = &self->orb;
	tx = (long)(p->boxRight + p->boxLeft) << 7;
	ty = (long)(p->boxTop + p->boxBottom) << 7;
	ox = orb->x;
	oy = orb->y;
	dx = tx - ox;
	dy = ty - oy;
	orb->vx = dx >> 4;
	orb->vy = dy >> 4;
	orb->x += orb->vx;
	orb->y += orb->vy;
	{
		register short px = orb->x >> 8;

		if (px < self->rangeLeft)
			orb->x = (long)self->rangeLeft << 8;
		else if (px > self->rangeRight)
			orb->x = (long)self->rangeRight << 8;
	}
	orb->vx = orb->x - ox;
	dx = orb->x - tx;
	return ABS(dx) + ABS(dy) < 0x1000;
}
