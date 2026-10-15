/* FarAcolyte_MoveBolt - FARACOLYTE slot +$7A: move the bolt one frame and step its animation
 * (frames 0-6, 4 frames each, then it stays on 6).
 *
 * self: the acolyte (FarAcolyte_StateBoltFly).
 * Returns 1 when the bolt is within 10 px (|dx| + |dy|) of the orb. */
#include "xmen2.h"

char FarAcolyte_MoveBolt(FarAcolyte *self)
{
	register FarAcolyteShot *orb;
	register FarAcolyteShot *bolt;
	long dx, dy;

	bolt = &self->bolt;
	orb = &self->orb;
	bolt->x += bolt->vx;
	bolt->y += bolt->vy;
	dx = orb->x - bolt->x;
	dy = orb->y - bolt->y;
	if (bolt->frame < 6 && !--bolt->frameTimer) {
		bolt->frameTimer = 4;
		bolt->frame++;
	}
	return ABS(dx) + ABS(dy) < 0xA00;
}
