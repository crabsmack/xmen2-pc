/* CapedAcolyte_StateReturnHome - CAPEDACOLYTE state after a knockback: fly vertically until the
 * home point's y is within 4 px of the position, then stop there and end the hurt (EndHurt,
 * slot +$76: CapedAcolyte_EndHurt).
 *
 * self: the object.
 * The speed is $180 down while home y is above the position, else up: away from home (a bug?).
 * As home moves with the velocity (CapedAcolyte_Move) and the position is home plus the hover
 * wobble, the distance is the wobble's and it soon ends anyway. No value returned. */
#include "xmen2.h"

void CapedAcolyte_StateReturnHome(register CapedAcolyte *self)
{
	register short x = self->x >> 8;	/* never used */
	register short y = self->homeY >> 8;
	register short d = ABS(y - self->py);

	if (d < 5) {
		self->homeY = (long)self->py << 8;
		STOP_MOVING(self);
		(*self->cls->method[28])(self);		/* +$76 EndHurt */
	} else
		self->vy = y < self->py ? 0x180 : -0x180;
}
