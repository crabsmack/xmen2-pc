/* BrainChild_ActFacePlayer - BRAINCHILD script action: stop and face the nearest player (sprite
 * flag $10: facing left), then the next step on the next update.
 *
 * self: the object (a step of its AI script; never runs in the corpus). */
#include "xmen2.h"

void BrainChild_ActFacePlayer(register BrainChild *self)
{
	XPlayer *p = Player_Nearest((Actor *)self);

	STOP_MOVING(self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	AI_NEXT_STEP(self, self->ai, 0);
}
