/* Wolverine_AirSpecial - WOLVERINE A in the air (class slot +$C6): the air lunge, vx $644 the way he
 * faces (maximum $644, no acceleration), then Wolverine_StateLunge from the next frame.
 *
 * self: the player.
 * Above 7 health units sound $8A and the strong lunge (Tbl_WolverineAnim11), else sound $88 and
 * Tbl_WolverineAnim10.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short Wolverine_AirSpecial(register Wolverine *self)
{
	self->vx = self->spr.flags & 0x10 ? -0x644 : 0x644;
	self->ax = 0;
	self->maxVx = 0x644;
	if (self->health >= 0x701) {
		Snd_Stop(0x8A);
		Snd_Play(0x8A);
		Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim11));
	} else {
		Snd_Stop(0x88);
		Snd_Play(0x88);
		Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim10));
	}
	SET_STATE(self, (StateFn)Wolverine_StateLunge);
	return self->flags & 8;
}
