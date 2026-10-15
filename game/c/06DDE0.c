/* Brood_StateCorneredFlash - BROOD state: flash (palette bit 1 toggled every 4 frames) until the
 * countdown runs out, then break out (Brood_StateBreakOut).
 *
 * self: the object. Returns flags & 8 when it breaks out, nothing while it flashes. */
#include "xmen2.h"

short Brood_StateCorneredFlash(Brood *self)
{
	if (!--self->flashTimer) {
		short pal = (self->spr.flags & 3) ^ 2;

		self->spr.flags = self->spr.flags & ~3 | pal;
		self->flashTimer = 4;
	}
	if (self->countdown == 0) {
		SET_STATE(self, Brood_StateBreakOut);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
