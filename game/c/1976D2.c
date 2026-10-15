/* Wolverine_GroundSpecial - WOLVERINE A on the ground (class slot +$DE): the leaping slash: off the
 * floor (Obj_Detach) with vy -$300, gravity $3F and vx $396 the way he faces (maximum $396, no
 * acceleration); Wolverine_LeapSlashEnd in 20 frames.
 *
 * self: the player.
 * Above 7 health units sound $8A and the strong slash (Tbl_WolverineAnim7), else sound $88 and
 * Tbl_WolverineAnim.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Wolverine_GroundSpecial(register Wolverine *self)
{
	Obj_Detach(self);
	self->vy = -0x300;
	self->ay = 0x3F;
	self->maxVx = 0x396;
	self->ax = 0;
	self->vx = self->spr.flags & 0x10 ? -0x396 : 0x396;
	if (self->health >= 0x701) {
		Snd_Stop(0x8A);
		Snd_Play(0x8A);
		Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim7));
	} else {
		Snd_Stop(0x88);
		Snd_Play(0x88);
		Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim));
	}
	SET_STATE_WAIT(self, (StateFn)Wolverine_LeapSlashEnd, 0x14);
	return self->flags & 8;
}
