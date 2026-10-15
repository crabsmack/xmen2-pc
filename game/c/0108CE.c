/* BigAcolyte_SetTargetY - start gliding a BIGACOLYTE vertically to y (BigAcolyte_Move), or mark
 * it arrived when it is there already.
 *
 * self: the boss.  y: target in pixels.
 * Stops the hover bob. Moving: acceleration $60 towards the target. Already there: arrived,
 * countdown 9. */
#include "xmen2.h"

void BigAcolyte_SetTargetY(register BigAcolyte *self, register short y)
{
	register short cur = self->y >> 8;

	self->bobbing = 0;
	if (cur != y) {
		self->targetY = y;
		self->arrivedY = 0;
		if (cur < y)
			self->ay = 0x60;
		else
			self->ay = -0x60;
	} else {
		self->arrivedY = 1;
		self->countdown = 9;
	}
}
