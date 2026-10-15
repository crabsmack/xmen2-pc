/* XPlayer_Steer - accelerate toward a side up to a maximum speed and drop any speed the other way,
 * without turning (XPlayer_StateFloatFall).
 *
 * self:  the player.
 * max:   maximum speed x.
 * accel: acceleration (toward the right when right, else toward the left).
 * right: 1 right, 0 left. */
#include "xmen2.h"

void XPlayer_Steer(XPlayer *self, short max, short accel, char right)
{
	self->ax = right ? accel : -accel;
	self->maxVx = max;
	if (right) {
		if (self->vx < 0)
			self->vx = 0;
	} else {
		if (self->vx > 0)
			self->vx = 0;
	}
}
