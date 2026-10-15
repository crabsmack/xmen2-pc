/* SavageBio_StateRetract - SAVAGEBIGBIO state: pull the tentacle in one segment per frame; with
 * the last one gone, frame $40 and go on with the script next frame (SavageBio_StateNextStep).
 *
 * self: the boss (after SavageBio_StateRetractStart).
 * Returns self->flags & 8. */
#include "xmen2.h"

short SavageBio_StateRetract(SavageBigBio *self)
{
	if (self->tentacleLen - 1 < 1) {
		(*self->cls->method[20])(self, 0x40);	/* SetFrame */
		self->tentacleLen = 0;
		self->tentacle = 0;
		SET_STATE(self, SavageBio_StateNextStep);
		return self->flags & 8;
	}
	self->tentacleLen--;
	SET_STATE(self, SavageBio_StateRetract);
	return self->flags & 8;
}
