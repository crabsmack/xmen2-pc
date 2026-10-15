/* XPlayer_StartFallOffScreen - start falling off the screen (XPlayer_StateFallOffScreen): 60 frames
 * unhittable; Cyclops ends its beam first.
 *
 * self: the player. Set by XPlayer_TetherToPartner.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short XPlayer_StartFallOffScreen(XPlayer *self)
{
	self->hitCooldown = 60;
	if (self->cls == CLASS(Tbl_Cyclops_vtable))
		Cyclops_EndBeam(self);
	SET_STATE(self, (StateFn)XPlayer_StateFallOffScreen);
	return self->flags & 8;
}
