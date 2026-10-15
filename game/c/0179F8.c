/* FarAcolyte_StartBolt - FARACOLYTE slot +$76: launch the bolt from the acolyte's hand (3 px to
 * the side it faces, 32 px up) towards the orb, reaching it in 16 frames.
 *
 * self: the acolyte (FarAcolyte_StateCast).
 * Bolt animation from frame 0, 4 frames per frame. */
#include "xmen2.h"

void FarAcolyte_StartBolt(FarAcolyte *self)
{
	register FarAcolyteShot *bolt;
	FarAcolyteShot *orb;

	bolt = &self->bolt;
	orb = &self->orb;
	bolt->x = self->x + ((!(self->spr.flags & 0x10) ? 3 : -3) << 8);
	bolt->y = self->y + -0x2000;
	bolt->vx = (orb->x - bolt->x) >> 4;
	bolt->vy = (orb->y - bolt->y) >> 4;
	bolt->frame = 0;
	bolt->frameTimer = 4;
	self->shotFlags |= 4;
}
