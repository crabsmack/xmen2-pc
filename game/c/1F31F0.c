/* XPlayer_StateHardLanding - after a hard landing: wait out the recovery (countdown) unless an
 * action is pressed (XPlayer_ActionPressed), then the control state (slot +$A6), now.
 *
 * self: the player. */
#include "xmen2.h"

void XPlayer_StateHardLanding(XPlayer *self)
{
	if (!self->countdown || XPlayer_ActionPressed(self))
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
}
