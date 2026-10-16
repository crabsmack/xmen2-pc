/* Tower_StateIdle - TOWER state: nothing to do; runs itself again in 255 frames.
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short Tower_StateIdle(register Tower *self)
{
	SET_STATE_WAIT(self, Tower_StateIdle, 0xFF);
	return self->flags & 8;
}
