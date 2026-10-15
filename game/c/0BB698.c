/* SavageBio_StateRetractStart - SAVAGEBIGBIO state (and script action): start pulling the
 * tentacle in (sound $F1, SavageBio_StateRetract next frame).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_StateRetractStart(SavageBigBio *self)
{
	Snd_Stop(0xF1);
	Snd_Play(0xF1);
	SET_STATE(self, SavageBio_StateRetract);
	return self->flags & 8;
}
