/* Obj_InScreenWindow - whether an object's body box overlaps the activation window around the
 * camera.
 *
 * self: the object.
 * flags6 $1000: always 1. Else its box minus the camera must reach into g_ActWinLeft..Right
 * (unless flags6 $800) and g_ActWinTop..Bottom (unless flags6 $20): 1, else 0. Like
 * Actor_InActivateWindow with the box instead of the position. */
#include "xmen2.h"

char Obj_InScreenWindow(Actor *self)
{
	short left = self->boxLeft - g_CameraX;
	short right = self->boxRight - g_CameraX;
	short top = self->boxTop - g_CameraY;
	short bottom = self->boxBottom - g_CameraY;

	if (self->flags6 & 0x1000)
		return 1;
	if (!(self->flags6 & 0x800)) {
		if (left > g_ActWinRight)
			return 0;
		if (right < g_ActWinLeft)
			return 0;
	}
	if (!(self->flags6 & 0x20)) {
		if (top > g_ActWinBottom)
			return 0;
		if (bottom < g_ActWinTop)
			return 0;
	}
	return 1;
}
