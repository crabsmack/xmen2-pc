/* MagnetoBoss_StateResumeShoot - MAGNETOBOSS: back from a shot during chase part 4, it shoots
 * again at once: MagnetoBoss_StateZone4Approach becomes the state to resume after it, then
 * MagnetoBoss_StateAimShoot (in shotDelay frames; at once for -1, which the unsigned byte
 * never is).
 *
 * self: the boss.
 * Returns self->flags & 8 after a delay, nothing otherwise. Never runs in the corpus. */
#include "xmen2.h"

short MagnetoBoss_StateResumeShoot(register MagnetoBoss *self)
{
	self->resumeState = MagnetoBoss_StateZone4Approach;
	if (self->shotDelay == -1)
		GOTO_STATE(self, MagnetoBoss_StateAimShoot);
	else {
		SET_STATE_WAIT(self, MagnetoBoss_StateAimShoot, self->shotDelay);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
