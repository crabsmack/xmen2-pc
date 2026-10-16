/* SavageMediumBio_ActFacePlayer - SAVAGEMEDIUMBIO script action: turn to face the nearest player,
 * go on with the script on the next update.
 *
 * self: the object (a step of its AI script). Returns nothing in particular. Never runs in the
 * corpus. */
#include "xmen2.h"

short SavageMediumBio_ActFacePlayer(register SavageMediumBio *self)
{
	register XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;	/* h-flip: face left */
	else
		self->spr.flags &= ~0x10;
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
