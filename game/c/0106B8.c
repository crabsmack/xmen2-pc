/* BigAcolyte_Draw - BIGACOLYTE Draw (class slot +$1A, replaces the parent's).
 *
 * self: the boss.
 * The hit blink of ACTOR Draw (flags6 bit $40, cleared once the hit cooldown is over: drawn on
 * odd g_LevelFrame only). As a picture: the scaled picture's sprites (Scaler_EmitSprites) at
 * the top left of its body box, palette from its sprite. Otherwise its sprite like ACTOR Draw
 * (nothing when no frame is shown; a distance-driven animation draws it animDist behind). */
#include "xmen2.h"

void BigAcolyte_Draw(register BigAcolyte *self)
{
	if (!self->hitCooldown)
		self->flags6 &= ~0x40;
	if ((self->flags6 & 0x40) && (g_LevelFrame & 1) == 0)
		return;
	if (self->picture) {
		Scaler_EmitSprites(self->boxLeft - g_CameraX, self->boxTop - g_CameraY, self->spr.flags & 3, 0);
		return;
	}
	if (self->spr.frame == 0xFF)
		return;
	Spr_Draw(&self->spr,
		self->anim && (*(u8 *)self->anim & 0x30) == 0x20
			? ((self->x - self->animDist) >> 8) - g_CameraX : (self->x >> 8) - g_CameraX,
		self->anim && (*(u8 *)self->anim & 0x30) == 0x30
			? ((self->y - self->animDist) >> 8) - g_CameraY : (self->y >> 8) - g_CameraY);
}
