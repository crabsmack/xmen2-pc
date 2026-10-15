/* XPlayer_StateDeathBlink - the dying player blinks: hidden for 10 frames, shown for px frames,
 * px growing by one every 20 frames; at 10 the death is over (XPlayer_DeathDone).
 *
 * self: the player (px, +$10, is the on-time here: XPlayer_StartDeath).
 * Returns self->flags & 8, as every state; nothing when the death ends. */
#include "xmen2.h"

short XPlayer_StateDeathBlink(XPlayer *self)
{
	if (self->countdown <= 0) {
		if (self->stateTimer <= 0) {
			self->px++;
			self->stateTimer = 20;
			if (self->px >= 10) {
				self->state = (StateFn)XPlayer_DeathDone;
				XPlayer_DeathDone(self);
				RETURN_NO_VALUE;
			}
		}
		(*self->cls->method[20])(self, self->charRec->hurtFrame);	/* SetFrame: shown */
		self->countdown = self->px;
		SET_STATE(self, (StateFn)XPlayer_StateDeathBlink);
		return self->flags & 8;
	} else {
		(*self->cls->method[20])(self, 0xFF);			/* SetFrame: hidden */
		SET_STATE(self, (StateFn)XPlayer_StateDeathBlink);
		return self->flags & 8;
	}
}
