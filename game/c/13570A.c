/* FlyBrood_WaitSwoopDone - FLYBROOD state after FlyBrood_StateSwoopPatrol: once the swoop path
 * has ended, turn around (headingRight flips, it faces that way) and take the next step on the next
 * update.
 *
 * self: the object. Returns flags & 8 while it waits (nothing in particular after).
 * Never runs in the corpus. */
#include "xmen2.h"

short FlyBrood_WaitSwoopDone(FlyBrood *self)
{
	if (!self->path) {
		if ((self->headingRight = self->headingRight ? 0 : 1))
			self->spr.flags &= ~0x10;
		else
			self->spr.flags |= 0x10;
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, FlyBrood_WaitSwoopDone);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
