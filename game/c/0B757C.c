/* ChildShip_StateReturnY - CHILDSHIP state (after a knockback): back to its height py at $100;
 * within 10 px, the cruise y is put on it, it stops, and its AI script goes on with the step it
 * was at (from the next update).
 *
 * self: the object.
 * The pixel x is computed and dropped (the original's). */
#include "xmen2.h"

void ChildShip_StateReturnY(ChildShip *self)
{
	register short x = self->x >> 8;
	register short y = self->homeY >> 8;
	register short dist = ABS(y - self->py);

	if (dist < 10) {
		self->homeY = self->py << 8;
		STOP_MOVING(self);
		self->state = FN_AT(StateFn, AI_SCRIPT(self->ai)->step + self->ai.index);
		self->timer = 0;
	} else
		self->vy = y < self->py ? 0x100 : -0x100;
}
