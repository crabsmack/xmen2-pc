/* MagnetoBoss_StateZone0 - MAGNETOBOSS chase, part 0: when the camera reaches x $1A4, hold it
 * in zone 1 ($1A4, $B0, speed 4) and hover there (MagnetoBoss_Zone1Setup); until then shoot at
 * the target each time the cooldown is over (MagnetoBoss_StateAimShoot: at once when shotDelay
 * is -1, which the unsigned byte never is, else in shotDelay frames).
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes after a delay, nothing otherwise. */
#include "xmen2.h"

short MagnetoBoss_StateZone0(register MagnetoBoss *self)
{
	if (g_CameraX >= 0x1A4) {
		AvalonA4_SetCameraZone(1, 0x1A4, 0xB0, 4, 4);
		SET_STATE(self, MagnetoBoss_Zone1Setup);
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
