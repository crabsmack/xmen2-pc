/* XPlayer_SteerFacing - steer in the air: accelerate toward a side up to a maximum speed, face that
 * side, and drop any speed the other way (XPlayer_AirSteer, XPlayer_StateDoubleJump).
 *
 * self:  the player.
 * max:   maximum speed x.
 * accel: acceleration (toward the right when right, else toward the left).
 * right: 1 right, 0 left. */
#include "xmen2.h"

void XPlayer_SteerFacing(XPlayer *self, short max, short accel, char right)
{
	self->ax = right ? accel : -accel;
	self->maxVx = max;
	if (right) {
		self->spr.flags &= ~0x10;
		if (self->vx < 0)
			self->vx = 0;
	} else {
		self->spr.flags |= 0x10;
		if (self->vx > 0)
			self->vx = 0;
	}
}
