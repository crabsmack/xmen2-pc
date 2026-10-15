/* Actor_InActivateWindow - ACTOR InActivateWindow (class slot +$3A): whether an object should
 * be active: inside the activation window around the camera and not riding another object.
 *
 * self: the object.
 * Attached to an object (spr.unk_0C not $FF): 0. flags6 $1000: always 1. Else its pixel
 * position minus the camera's must be within g_ActWinLeft..Right (unless flags6 $800) and
 * g_ActWinTop..Bottom (unless flags6 $20): 1, else 0. */
#include "xmen2.h"

char Actor_InActivateWindow(Actor *self)
{
	if (self->spr.unk_0C != 0xFF)
		return 0;
	if (self->flags6 & 0x1000)
		return 1;
	if (!(self->flags6 & 0x800)) {
		short dx = (self->x >> 8) - g_CameraX;

		if (dx > g_ActWinRight)
			return 0;
		if (dx < g_ActWinLeft)
			return 0;
	}
	if (!(self->flags6 & 0x20)) {
		short dy = (self->y >> 8) - g_CameraY;

		if (dy > g_ActWinBottom)
			return 0;
		if (dy < g_ActWinTop)
			return 0;
	}
	return 1;
}
