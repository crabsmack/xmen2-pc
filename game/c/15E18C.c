/* Actor_OutsideActiveWindow - ACTOR OutsideActiveWindow (class slot +$42): whether an active
 * object has left the active window around the camera (the level loop then Deactivates it).
 *
 * self: the object.
 * Never while it rides another object (spr.parent not $FF), nor with flags6 $1000 while it has
 * health. Else 1 when its pixel position minus the camera's is outside g_DeactWinLeft..Right
 * (unless flags6 $800) or g_DeactWinTop..Bottom (unless flags6 $20), else 0. */
#include "xmen2.h"

char Actor_OutsideActiveWindow(Actor *self)
{
	if (self->spr.parent != 0xFF)
		return 0;
	if ((self->flags6 & 0x1000) && self->health > 0)
		return 0;
	if (!(self->flags6 & 0x800)) {
		short dx = (self->x >> 8) - g_CameraX;

		if (dx > g_DeactWinRight)
			return 1;
		if (dx < g_DeactWinLeft)
			return 1;
	}
	if (!(self->flags6 & 0x20)) {
		short dy = (self->y >> 8) - g_CameraY;

		if (dy > g_DeactWinBottom)
			return 1;
		if (dy < g_DeactWinTop)
			return 1;
	}
	return 0;
}
