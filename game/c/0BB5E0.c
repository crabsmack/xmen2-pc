/* SavageBio_StateExtend - SAVAGEBIGBIO state: grow the tentacle one segment per frame until the
 * next point is the path's end, then hold it out $14 frames and retract it
 * (SavageBio_StateRetractStart).
 *
 * self: the boss (after SavageBio_ActExtendTentacle/2; never runs in the corpus).
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_StateExtend(SavageBigBio *self)
{
	if ((self->tentacle + (self->tentacleLen + 1))->x >= 1000) {	/* the next point ends the path */
		SET_STATE_WAIT(self, SavageBio_StateRetractStart, 0x14);
		return self->flags & 8;
	}
	self->tentacleLen++;
	SET_STATE(self, SavageBio_StateExtend);
	return self->flags & 8;
}
