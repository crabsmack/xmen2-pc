/* MagnetoBoss_StateFinalFight - MAGNETOBOSS final fight: shoot at the target each time the
 * cooldown is over (MagnetoBoss_StateAimShoot, in shotDelay frames; at once for -1, which the
 * unsigned byte never is). THRONEPHONE ends it (MagnetoBoss_EnterSpinState).
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes after a delay, nothing otherwise. */
#include "xmen2.h"

short MagnetoBoss_StateFinalFight(register MagnetoBoss *self)
{
	if (self->target && !self->shotCooldown) {
		self->shotCooldown = self->shotPeriod;
		if (self->shotDelay == -1)
			GOTO_STATE(self, MagnetoBoss_StateAimShoot);
		else {
			SET_STATE_WAIT(self, MagnetoBoss_StateAimShoot, self->shotDelay);
			return self->flags & 8;
		}
	}
	NO_RETURN_VALUE
}
