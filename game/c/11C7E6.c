/* BabyBio_ActFacePlayer - BABYBIO AI-script action: stop, turn to the nearest player (its flame
 * too), next step now.
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_ActFacePlayer(BabyBio *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	STOP_MOVING(self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	if (self->flame) {
		if (self->spr.flags & 0x10)
			self->flame->spr.flags |= 0x10;
		else
			self->flame->spr.flags &= ~0x10;
	}
	AI_NEXT_STEP(self, self->ai, 0);
}
