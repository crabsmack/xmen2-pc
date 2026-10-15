/* Tusk_StateReturnHome - TUSK walking home: when its speed is more than the distance left, stop
 * (Tusk_StateAtHome).
 *
 * self: TUSK.
 * Returns 0 when it switches; no value while walking. */
#include "xmen2.h"

short Tusk_StateReturnHome(Tusk *self)
{
	long dx;

	dx = (self->px << 8) - self->x;
	if (ABS(dx) < ABS(self->vx)) {
		self->ax = 0;
		self->vx = 0;
		self->state = Tusk_StateAtHome;
		return 0;
	}
	NO_RETURN_VALUE
}
