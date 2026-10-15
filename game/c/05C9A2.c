/* Tusk_StateCharging - TUSK charging: once more than $43 px from home, stop accelerating and
 * brake (speed 800 backwards) for 7 frames (Tusk_StateChargeStop).
 *
 * self: TUSK.
 * Returns 0 when it switches; no value while charging. */
#include "xmen2.h"

short Tusk_StateCharging(Tusk *self)
{
	long dx;

	dx = (self->px << 8) - self->x;
	if (ABS(dx) > 0x4300) {
		self->ax = 0;
		self->vx = 0;
		self->vx = -(self->spr.flags & 0x10 ? -800 : 800);
		self->state = Tusk_StateChargeStop;
		self->timer = 7;
		return 0;
	}
	NO_RETURN_VALUE
}
