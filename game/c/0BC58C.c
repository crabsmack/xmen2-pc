/* SavageBio_StateWaitCrossing - SAVAGEBIGBIO state: wait until the BIOBALLS companion is done
 * crossing, hide it, and reappear on the other surface (floor <-> ceiling, v-flip toggled);
 * then go on with the script.
 *
 * self: the boss (after SavageBio_StateCrossSurface; never runs in the corpus).
 * With no companion it crosses at once.
 * Returns self->flags & 8 while waiting; nothing in particular when the script goes on. */
#include "xmen2.h"

short SavageBio_StateWaitCrossing(SavageBigBio *self)
{
	char done = 1;

	if (self->balls && (done = BioBalls_IsDone(self->balls)))
		(*self->balls->cls->method[20])(self->balls, 0xFF);	/* SetFrame: hide */
	if (done) {
		if (!(self->spr.flags & 8))
			self->y = self->arenaCeiling << 8;
		else
			self->y = self->arenaFloor << 8;
		self->spr.flags ^= 8;
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE(self, SavageBio_StateWaitCrossing);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
