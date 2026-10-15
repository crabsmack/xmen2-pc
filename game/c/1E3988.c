/* Cyclops_GroundAim - CYCLOPS stop and aim the beam standing (Cyclops_StateGroundAim, now).
 *
 * self: the player.
 * No speed or acceleration x, no animation. */
#include "xmen2.h"

void Cyclops_GroundAim(Cyclops *self)
{
	self->ax = 0;
	self->vx = 0;
	self->anim = 0;
	GOTO_STATE(self, Cyclops_StateGroundAim);
}
