/* MagnetoBoss_StateWaitCamera - MAGNETOBOSS: when the camera reaches x $750, hold it in zone 5
 * ($750, $B0, speed 8) and start the final fight (MagnetoBoss_FinalSetup).
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes, nothing otherwise. */
#include "xmen2.h"

short MagnetoBoss_StateWaitCamera(register MagnetoBoss *self)
{
	if (g_CameraX >= 0x750) {
		AvalonA4_SetCameraZone(5, 0x750, 0xB0, 8, 8);
		SET_STATE(self, MagnetoBoss_FinalSetup);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
