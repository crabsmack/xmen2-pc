/* MagneticGlob_OutsideActiveWindow - MAGNETICGLOB OutsideActiveWindow (class slot +$42): whether
 * the globe is well off the screen: more than 50 px left or above it, 50 px right of it (x > 370)
 * or 51 px below (y > 274).
 *
 * self: the globe.  Returns 1 outside, else 0. */
#include "xmen2.h"

char MagneticGlob_OutsideActiveWindow(register MagneticGlob *self)
{
	register short v = (self->x >> 8) - g_CameraX;

	if (v > 0x172)
		return 1;
	if (v < -0x32)
		return 1;
	v = (self->y >> 8) - g_CameraY;
	if (v > 0x112)
		return 1;
	if (v < -0x32)
		return 1;
	return 0;
}
