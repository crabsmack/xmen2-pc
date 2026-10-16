/* ScalingActor_Draw - SCALINGACTOR class slot +$1A (the parent's isn't called): while its slot is
 * a big sprite (flying), the scaled picture (Cerebro_BigSpriteDraw); else, with a sprite
 * definition and the slot not hidden, the character's sprite at its screen position (no hit
 * blink).
 *
 * self: the portrait. */
#include "xmen2.h"

void ScalingActor_Draw(register ScalingActor *self)
{
	if (self->slot->big)
		Cerebro_BigSpriteDraw(self->slot);
	else if (self->spr.def && !self->slot->hidden)
		Spr_Draw(&self->spr, (self->x >> 8) - g_CameraX, (self->y >> 8) - g_CameraY);
}
