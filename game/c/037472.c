/* MagnetoBoss_StateZone4Approach - MAGNETOBOSS chase, part 4: when the camera reaches x $600,
 * hold it in zone 4 ($600, $B0, speed 8) and hover there (MagnetoBoss_Zone4Hover); until then
 * shoot at the target each time the cooldown is over (MagnetoBoss_StateAimShoot, in shotDelay
 * frames; at once for -1, which the unsigned byte never is), coming back through
 * MagnetoBoss_StateResumeShoot.
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes after a delay, nothing otherwise. */
#include "xmen2.h"

short MagnetoBoss_StateZone4Approach(register MagnetoBoss *self)
{
	if (g_CameraX >= 0x600) {
		AvalonA4_SetCameraZone(4, 0x600, 0xB0, 8, 8);
		SET_STATE(self, MagnetoBoss_Zone4Hover);
		return self->flags & 8;
	}
	if (self->target && !self->shotCooldown) {
		self->resumeState = MagnetoBoss_StateResumeShoot;
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
