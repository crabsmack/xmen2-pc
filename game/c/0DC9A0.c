/* Barrier_StateIdle - BARRIER state once closed: nothing; runs itself again next frame.
 *
 * self: the barrier. Returns self->flags & 8. */
#include "xmen2.h"

short Barrier_StateIdle(register Barrier *self)
{
	SET_STATE(self, Barrier_StateIdle);
	return self->flags & 8;
}
