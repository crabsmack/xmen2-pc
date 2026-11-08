/* MagnetoBoss_StateWaitCamera - MAGNETOBOSS: when the camera reaches x $750, hold it in zone 5
 * ($750, $B0, speed 8) and start the final fight (MagnetoBoss_FinalSetup).
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes, nothing otherwise.
 * NATIVE: $750 is within 16 px of the Throne Room's camera limit ($760); with widescreen_gameplay the
 * camera stops at CAM_HI ($760 less the margin, $738 at 16:9) and never got there, so Magneto hovered
 * in this state for good (docs/NOTES.md "Magneto's last phase"). The threshold and the zone's x are
 * $750 or CAM_HI, whichever is less: $750 with the option off. */
#include "xmen2.h"

short MagnetoBoss_StateWaitCamera(register MagnetoBoss *self)
{
#if defined(XMEN2_NATIVE)
	short x = 0x750;

	if (x > CAM_HI(g_CamMinX, g_CamMaxX))
		x = CAM_HI(g_CamMinX, g_CamMaxX);
	if (g_CameraX >= x) {
		AvalonA4_SetCameraZone(5, x, 0xB0, 8, 8);
		SET_STATE(self, MagnetoBoss_FinalSetup);
		return self->flags & 8;
	}
#else
	if (g_CameraX >= 0x750) {
		AvalonA4_SetCameraZone(5, 0x750, 0xB0, 8, 8);
		SET_STATE(self, MagnetoBoss_FinalSetup);
		return self->flags & 8;
	}
#endif
	NO_RETURN_VALUE
}
