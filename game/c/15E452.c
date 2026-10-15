/* Actor_DrawSprite - ACTOR DrawIfActive (class slot +$1E): draw the object as Actor_Draw does,
 * or, while it is inactive (flags $4000), only place the objects attached to it.
 *
 * self: the object (Spr_PlaceAttached calls this slot on attached objects).
 * Inactive: Spr_PlaceAttached(&spr, 0, 0) when it carries objects. Else blinking (flags6 $40)
 * skips even frames (g_LevelFrame bit 0), and the sprite is drawn at its pixel position minus
 * the camera, animDist back along a distance-driven animation's axis (script flags & $30: $20 x,
 * $30 y). */
#include "xmen2.h"

void Actor_DrawSprite(Actor *self)
{
	if ((self->flags & 0x4000) == 0) {
		if ((self->flags6 & 0x40) && (g_LevelFrame & 1) == 0)
			return;
		Spr_Draw(&self->spr,
			self->anim && (((AnimScript *)self->anim)->flags & 0x30) == 0x20
				? ((self->x - self->animDist) >> 8) - g_CameraX : (self->x >> 8) - g_CameraX,
			self->anim && (((AnimScript *)self->anim)->flags & 0x30) == 0x30
				? ((self->y - self->animDist) >> 8) - g_CameraY : (self->y >> 8) - g_CameraY);
	} else if (self->spr.firstChild != 0xFF)
		Spr_PlaceAttached(&self->spr, 0, 0);
}
