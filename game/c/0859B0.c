/* BroodBossParts_Draw - BROODBOSSPARTS Draw (class slot +$1A): ACTOR's Draw with a wider window:
 * the part is drawn only while its x is more than $40 px left of the camera's and less than $180
 * px right of it (the boss is larger than the screen).
 *
 * self: the part.
 * Blinking (flags6 $40): drawn on odd frames only (g_LevelFrame bit 0). Position: pixels minus
 * the camera; with a distance-driven animation (script flags & $30: $20 in x, $30 in y) the
 * sprite is drawn animDist back along that axis. */
#include "xmen2.h"

void BroodBossParts_Draw(BroodBossParts *self)
{
	short dx = (self->x >> 8) - g_CameraX;

	if (dx <= -0x40 || dx >= 0x180)
		return;
	if ((self->flags6 & 0x40) && (g_LevelFrame & 1) == 0)
		return;
	Spr_Draw(&self->spr,
		self->anim && (((AnimScript *)self->anim)->flags & 0x30) == 0x20
			? ((self->x - self->animDist) >> 8) - g_CameraX : (self->x >> 8) - g_CameraX,
		self->anim && (((AnimScript *)self->anim)->flags & 0x30) == 0x30
			? ((self->y - self->animDist) >> 8) - g_CameraY : (self->y >> 8) - g_CameraY);
}
