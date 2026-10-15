/* BanimazaDoor_InActivateWindow - BANIMAZADOOR slot +$3A: whether the door is close enough to
 * the screen to activate: its pixel x - g_CameraX within -$20..$160 and py - g_CameraY + $40
 * within -$20..$100.
 *
 * self: the door. Returns 1 inside, 0 not. */
#include "xmen2.h"

char BanimazaDoor_InActivateWindow(register BanimazaDoor *self)
{
	register long d = self->px - g_CameraX;

	if (d < -32)
		return 0;
	if (d > 0x160)
		return 0;
	d = self->py - g_CameraY + 0x40;
	if (d < -32)
		return 0;
	if (d > 0x100)
		return 0;
	return 1;
}
