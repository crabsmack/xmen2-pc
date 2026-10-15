/* Deathbird_StateSwoopDown - DEATHBIRD swoop (SetMoveMode 5): once 120 px below the top of the
 * screen, stop (SetMoveMode 3) and pause 8 frames (Deathbird_StateHoverPause).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateSwoopDown(Deathbird *self)
{
	if ((self->y >> 8) >= g_CameraY + 120) {
		(*self->cls->method[27])(self, 3);	/* SetMoveMode */
		self->countdown = 8;
		SET_STATE(self, (StateFn)Deathbird_StateHoverPause);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)Deathbird_StateSwoopDown);
	return self->flags & 8;
}
