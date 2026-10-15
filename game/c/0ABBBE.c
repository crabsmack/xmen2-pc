/* FloatBubble_OutsideActiveWindow - FLOATBUBBLE OutsideActiveWindow (class slot +$42): is the
 * bubble out of the active window around the camera?
 *
 * self: the bubble.
 * Returns 1 when its position relative to the camera's top-left is more than $13A px below or
 * right, more than $10 px above or more than $64 px left (y tested first), else 0. */
#include "xmen2.h"

char FloatBubble_OutsideActiveWindow(FloatBubble *self)
{
	long d;

	d = (self->y >> 8) - g_CameraY;
	if (d > 0x13A)
		return 1;
	if (d < -0x10)
		return 1;
	d = (self->x >> 8) - g_CameraX;
	if (d > 0x13A)
		return 1;
	if (d < -0x64)
		return 1;
	return 0;
}
