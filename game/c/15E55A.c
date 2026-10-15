/* Actor_Draw - ACTOR Draw (class slot +$1A, what Actor_Activate puts in the draw list): draw the
 * object's sprite where it is on screen.
 *
 * self: the object.
 * Blinking (flags6 $40): drawn on odd frames only (g_LevelFrame bit 0). Position: pixels minus
 * the camera; with a distance-driven animation (script flags & $30: $20 in x, $30 in y) the
 * sprite is drawn animDist back along that axis. */
#include "xmen2.h"

void Actor_Draw(Actor *self)
{
	short unused;	/* d7, saved and never used: a variable whose code was compiled out */

	if (0)
		unused = 0;
	if ((self->flags6 & 0x40) && (g_LevelFrame & 1) == 0)
		return;
	Spr_Draw(&self->spr,
		self->anim && (((AnimScript *)self->anim)->flags & 0x30) == 0x20
			? ((self->x - self->animDist) >> 8) - g_CameraX : (self->x >> 8) - g_CameraX,
		self->anim && (((AnimScript *)self->anim)->flags & 0x30) == 0x30
			? ((self->y - self->animDist) >> 8) - g_CameraY : (self->y >> 8) - g_CameraY);
}
