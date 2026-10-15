/* BanimazaDoor_OutsideActiveWindow - BANIMAZADOOR slot +$42: whether the door is far enough off
 * screen to deactivate: never while it is kept active (flags6 $1000); else when its pixel x -
 * g_CameraX leaves -$40..$180 or py - g_CameraY + $40 leaves -$40..$120.
 *
 * self: the door. Returns 1 outside, 0 not. */
#include "xmen2.h"

char BanimazaDoor_OutsideActiveWindow(register BanimazaDoor *self)
{
	register long d;

	if (self->flags6 & 0x1000)
		return 0;
	d = self->px - g_CameraX;
	if (d < -64)
		return 1;
	if (d > 0x180)
		return 1;
	d = self->py - g_CameraY + 0x40;
	if (d < -64)
		return 1;
	if (d > 0x120)
		return 1;
	return 0;
}
