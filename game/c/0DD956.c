/* ControlBox_StateArm - CONTROLBOX state: the first blast in 10 frames, then ControlBox_StateFire
 * from the next update.
 *
 * self: the box. Returns self->flags & 8. */
#include "xmen2.h"

short ControlBox_StateArm(register ControlBox *self)
{
	self->fireTimer = 10;
	SET_STATE(self, (StateFn)ControlBox_StateFire);
	return self->flags & 8;
}
