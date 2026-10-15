/* SavageBio_StateToCentre - SAVAGEBIGBIO state: walk (speed $220) toward the middle of the view
 * (camera x + $A0); within 15 px stop and go on with the script.
 *
 * self: the boss (after SavageBio_ActToCentre).
 * Only the acceleration and the horizontal speed are cleared at the end.
 * Returns self->flags & 8 while walking; nothing in particular when the script goes on. */
#include "xmen2.h"

short SavageBio_StateToCentre(SavageBigBio *self)
{
	short x = self->x >> 8;
	short centre = g_CameraX + 0xA0;

	if (ABS(x - centre) < 0xF) {
		self->ax = 0;
		self->vx = 0;
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		if (x < centre)
			self->spr.flags &= ~0x10;
		else
			self->spr.flags |= 0x10;
		self->vx = (self->spr.flags & 0x10) ? -0x220 : 0x220;
		SET_STATE(self, SavageBio_StateToCentre);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
