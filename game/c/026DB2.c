/* RetractFloor_StateWait - RETRACTFLOOR state: wait to be started. Until some floor has dropped
 * (g_FloatFallSpeed 0), the camera's dead zone keeps the players between screen y 100 and $91.
 * Once started: flags6 |= $1000, each drop goes $10 deeper (g_RetractFloorDepth, up to $500),
 * and it drops in 30 frames (RetractFloor_StateDrop).
 *
 * self: the object.
 * Returns flags bit 3 once started; no value while waiting. */
#include "xmen2.h"

short RetractFloor_StateWait(register RetractFloor *self)
{
	if (!g_FloatFallSpeed) {
		g_Camera->deadTop = 100;
		g_Camera->deadBottom = 0x91;
	}
	if (self->triggered & 1) {
		self->flags6 |= 0x1000;
		if (g_RetractFloorDepth < 0x500)
			g_RetractFloorDepth += 0x10;
		SET_STATE_WAIT(self, RetractFloor_StateDrop, 30);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
