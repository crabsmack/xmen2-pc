/* XPlayer_WalkAccel - accelerate toward a side up to a maximum speed, and face the way it moves.
 *
 * self:  the player.
 * max:   maximum speed x.
 * accel: acceleration (toward the right when right, else toward the left).
 * right: 1 right, 0 left.
 * The facing (sprite h-flip, $10 = left) follows vx, not the side asked. */
#include "xmen2.h"

void XPlayer_WalkAccel(XPlayer *self, short max, short accel, char right)
{
	self->ax = right ? accel : -accel;
	self->maxVx = max;
	if (self->vx < 0)
		self->spr.flags |= 0x10;
	if (self->vx > 0)
		self->spr.flags &= ~0x10;
}
