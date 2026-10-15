/* FarAcolyte_StartOrb - FARACOLYTE slot +$6E (its own): put the orb out, 3 px right of and
 * 32 px above the acolyte, at rest; it doesn't flicker yet.
 *
 * self: the acolyte (FarAcolyte_StateWaitPlayer). */
#include "xmen2.h"

void FarAcolyte_StartOrb(FarAcolyte *self)
{
	register FarAcolyteShot *orb;

	orb = &self->orb;
	orb->x = self->x + 0x300;
	orb->y = self->y + -0x2000;
	orb->vx = orb->vy = orb->frame = 0;
	self->shotFlags |= 1;
	self->shotFlags &= ~2;
}
