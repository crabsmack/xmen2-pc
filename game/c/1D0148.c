/* Gambit_CrouchAim - stop, crouch with the aiming animation (Tbl_Anim_1CEE42) and aim cards low
 * (Gambit_StateCrouchAim, now).
 *
 * self: the player (Down+A on the ground: Gambit_GroundSpecialDown). */
#include "xmen2.h"

void Gambit_CrouchAim(register Gambit *self)
{
	self->ax = 0;
	self->vx = 0;
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1CEE42));
	self->state = Gambit_StateCrouchAim;
	Gambit_StateCrouchAim(self);
end:	;	/* (here MPW C pops all the arguments, as the ROM does) */
}
