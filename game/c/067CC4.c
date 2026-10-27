/* PortraitActor_StateLanded - SCALINGACTOR state: the portrait has flown in: no longer a big
 * sprite, the slot no longer busy, the character's sprite set up again (Spr_Init, no frame yet;
 * facing left for player 2), its pick pose and sound (PlayerSelect_PlayPickAnim), standing at its
 * player's side. Then PortraitActor_StateIdle from the next update.
 *
 * self: the portrait. Returns self->flags & 8. */
#include "xmen2.h"

short PortraitActor_StateLanded(register ScalingActor *self)
{
	self->slot->big = 0;
	self->slot->busy = 0;
	Spr_Init(&self->spr, 1, self->spr.def, -1, self->spr.flags & 3, self->spr.tileBase, 0);
	if (SELECT_FACES_LEFT(self))
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	PlayerSelect_PlayPickAnim(self);
	self->x = (self->homeX + g_CameraX) << 8;
	self->y = (g_CameraY + SELECT_HOME_Y) << 8;
	SET_STATE(self, PortraitActor_StateIdle);
	return self->flags & 8;
}
