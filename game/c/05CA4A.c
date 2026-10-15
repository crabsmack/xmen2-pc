/* Tusk_StateReturnStart - TUSK walks back home: speed 900 against the way it faces, speed limit
 * that speed; then Tusk_StateReturnHome.
 *
 * self: TUSK.
 * Returns 0. */
#include "xmen2.h"

short Tusk_StateReturnStart(Tusk *self)
{
	self->vx = -(self->spr.flags & 0x10 ? -900 : 900);
	self->maxVx = ABS(self->vx);
	self->state = Tusk_StateReturnHome;
	return 0;
}
