/* MagnetoBoss_StateZone2Approach - MAGNETOBOSS chase, part 2: when the camera reaches x $39F,
 * hold it in zone 2 ($39F, $B0, speed 4) and hover there (MagnetoBoss_Zone2Hover); until then
 * shoot at the target each time the cooldown is over (MagnetoBoss_StateAimShoot, in shotDelay
 * frames; at once for -1, which the unsigned byte never is).
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes after a delay, nothing otherwise. */
#include "xmen2.h"

short MagnetoBoss_StateZone2Approach(register MagnetoBoss *self)
{
	if (g_CameraX >= 0x39F) {
		AvalonA4_SetCameraZone(2, 0x39F, 0xB0, 4, 4);
		SET_STATE(self, MagnetoBoss_Zone2Hover);
		return self->flags & 8;
	}
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
