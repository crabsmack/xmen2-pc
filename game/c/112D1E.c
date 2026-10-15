/* Deathbird_StateDiveLow - DEATHBIRD low pass (SetMoveMode 7): once more than 100 px below the
 * top of the screen, pull up (SetMoveMode 8), throw the burst (its timer started, sound $FC) and
 * fly off (Deathbird_StateFlyOff).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateDiveLow(Deathbird *self)
{
	if ((self->y >> 8) - g_CameraY > 100) {
		(*self->cls->method[27])(self, 8);	/* SetMoveMode */
		(*self->cls->method[26])(self);		/* LaunchBurst */
		Timer_Start(self->burstTimer);
		Snd_Stop(0xFC);
		Snd_Play(0xFC);
		SET_STATE(self, (StateFn)Deathbird_StateFlyOff);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)Deathbird_StateDiveLow);
	return self->flags & 8;
}
