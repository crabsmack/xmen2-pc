/* Apocalypse_StateKnockback - APOCALYPSE knockback: halve the horizontal speed every frame; once
 * below $30, stop, 60 frames of hit cooldown and resume the AI script
 * (Apocalypse_StateResumeScript) when the countdown has run out (now if it is -1).
 *
 * self: the boss.
 * Returns self->flags & 8, nothing when it resumes the script now. */
#include "xmen2.h"

short Apocalypse_StateKnockback(register Apocalypse *self)
{
	if (ABS(self->vx) < 0x30) {
		self->hitCooldown = 60;
		STOP_MOVING(self);
		if (self->countdown == -1) {
			GOTO_STATE(self, (StateFn)Apocalypse_StateResumeScript);
		} else {
			SET_STATE_WAIT(self, (StateFn)Apocalypse_StateResumeScript, self->countdown);
			return self->flags & 8;
		}
	} else {
		self->vx = self->vx >> 1;
		SET_STATE(self, Apocalypse_StateKnockback);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
