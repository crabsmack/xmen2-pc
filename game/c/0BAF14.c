/* SavageBio_ActFacePlayer - SAVAGEBIGBIO script action: stop, turn to face the nearest player,
 * go on with the script on the next update.
 *
 * self: the boss (a step of its AI script; also a state, after SavageBio_ActDashTo).
 * Returns nothing in particular. */
#include "xmen2.h"

short SavageBio_ActFacePlayer(SavageBigBio *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	STOP_MOVING(self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;	/* h-flip: face left */
	else
		self->spr.flags &= ~0x10;
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
