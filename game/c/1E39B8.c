/* Cyclops_CrouchAim - CYCLOPS stop, crouch to aim (Tbl_Anim_1E1C48) and aim the beam low
 * (Cyclops_StateCrouchAim, now).
 *
 * self: the player.
 * The state change is not the GOTO_STATE macro: no argument pop between the calls, both at the
 * end. */
#include "xmen2.h"

void Cyclops_CrouchAim(Cyclops *self)
{
	self->ax = 0;
	self->vx = 0;
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1E1C48));
	if (1) {	/* (a block under a constant condition: MPW pops the arguments at its end) */
		self->state = Cyclops_StateCrouchAim;
		Cyclops_StateCrouchAim(self);
	}
}
