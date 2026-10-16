/* CapedAcolyte_ActFacePlayer - CAPEDACOLYTE script action: turn to face the nearest player (h-flip
 * = facing left), then the next script step in 10 frames.
 *
 * self: the object.
 * It turns only when the player is on the side it isn't facing; level with it, it stays as it
 * is unless it faced left (then it turns right). */
#include "xmen2.h"

void CapedAcolyte_ActFacePlayer(register CapedAcolyte *self)
{
	register XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (!(self->spr.flags & 0x10 ? p->x < self->x : p->x > self->x)) {
		if (p->x < self->x)
			self->spr.flags |= 0x10;
		else
			self->spr.flags &= ~0x10;
	}
	AI_NEXT_STEP(self, self->ai, 10);
}
