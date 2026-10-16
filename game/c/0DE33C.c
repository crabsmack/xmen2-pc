/* ControlPanel_StateArm - CONTROLPANEL state: the first blast in 10 frames, then
 * ControlPanel_StateFire from the next update.
 *
 * self: the panel.
 * Returns self->flags & 8. */
#include "xmen2.h"

short ControlPanel_StateArm(register ControlPanel *self)
{
	self->fireTimer = 10;
	SET_STATE(self, (StateFn)ControlPanel_StateFire);
	return self->flags & 8;
}
