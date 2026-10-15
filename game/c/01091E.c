/* BigAcolyte_SetTargetX - start gliding a BIGACOLYTE horizontally to x (BigAcolyte_Move), or mark
 * it arrived when it is there already.
 *
 * self: the boss.  x: target in pixels.
 * Stops the hover bob. Moving: acceleration $60 towards the target. Already there: arrived,
 * countdown 9. */
#include "xmen2.h"

void BigAcolyte_SetTargetX(register BigAcolyte *self, register short x)
{
	register short cur = self->x >> 8;

	self->bobbing = 0;
	if (cur != x) {
		self->targetX = x;
		self->arrivedX = 0;
		if (cur < x)
			self->ax = 0x60;
		else
			self->ax = -0x60;
	} else {
		self->arrivedX = 1;
		self->countdown = 9;
	}
}
