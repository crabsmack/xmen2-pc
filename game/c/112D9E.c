/* Deathbird_StateGoToSwitch - DEATHBIRD flying at the lift switch (SetMoveMode 9): once within 60
 * px in x and 40 px in y of (240, 408), land on it (Deathbird_StateLandOnSwitch); if it flies more
 * than 60 px past the screen edge it is heading for, give up and fly off (SetMoveMode 4,
 * Deathbird_StateFlyOff).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateGoToSwitch(Deathbird *self)
{
	if (ABS((self->x >> 8) - 0xF0) < 60 && ABS((self->y >> 8) - 0x198) < 40) {
		SET_STATE(self, (StateFn)Deathbird_StateLandOnSwitch);
		return self->flags & 8;
	}
	if (!(self->spr.flags & 0x10) && (self->x >> 8) - 60 > g_CameraX + 320
	    || self->spr.flags & 0x10 && (self->x >> 8) + 60 < g_CameraX) {
		(*self->cls->method[27])(self, 4);	/* SetMoveMode */
		SET_STATE(self, (StateFn)Deathbird_StateFlyOff);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)Deathbird_StateGoToSwitch);
	return self->flags & 8;
}
