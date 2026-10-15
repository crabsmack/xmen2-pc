/* Magneto_GroundAttackDown - MAGNETO Down+B on the ground (class slot +$DA): stop, crouch to aim
 * (Tbl_MagnetoAnim26) and aim low (Magneto_StateCrouchAim, now).
 *
 * self: the player.
 * The state change is not the GOTO_STATE macro: no argument pop between the calls, both at the
 * end. */
#include "xmen2.h"

void Magneto_GroundAttackDown(Magneto *self)
{
	self->ax = 0;
	self->vx = 0;
	Obj_SetAnim(self, CODEPTR(Tbl_MagnetoAnim26));
	if (1) {	/* (a block under a constant condition: MPW pops the arguments at its end) */
		self->state = Magneto_StateCrouchAim;
		Magneto_StateCrouchAim(self);
	}
}
