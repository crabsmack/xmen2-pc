/* Deathbird_StateFlyOff - DEATHBIRD state while it leaves: once more than 60 px past the left or
 * right edge of the screen, wait off screen (SetMoveMode 6: the side it left by), stop the burst
 * and choose the next attack (Deathbird_StateChooseAttack).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateFlyOff(Deathbird *self)
{
	if ((self->x >> 8) + 60 < g_CameraX || (self->x >> 8) - 60 > g_CameraX + 320) {
		(*self->cls->method[27])(self, 6);	/* SetMoveMode */
		Timer_Stop(self->burstTimer);
		SET_STATE(self, (StateFn)Deathbird_StateChooseAttack);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)Deathbird_StateFlyOff);
	return self->flags & 8;
}
