/* Exodus_StateRiseEnd - EXODUS state: end the rise: sway angles back to 0, x back on its pixel,
 * x stops moving, and float down again in 55 frames (Exodus_StateDescend).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Exodus_StateRiseEnd(register Exodus *self)
{
	self->swayAngleX = 0;
	self->swayAngleY = 0;
	self->x = self->px << 8;
	self->moveFlags &= ~2;
	SET_STATE_WAIT(self, Exodus_StateDescend, 55);
	return self->flags & 8;
}
