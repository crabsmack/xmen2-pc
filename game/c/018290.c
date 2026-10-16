/* ShootAcolyte_ActFacePlayer - SHOOTACOLYTE script action: face the nearest player (h-flip =
 * facing left; level with it: right), then the next script step in 20 frames.
 *
 * self: the object. */
#include "xmen2.h"

void ShootAcolyte_ActFacePlayer(register ShootAcolyte *self)
{
	register XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	AI_NEXT_STEP(self, self->ai, 20);
}
