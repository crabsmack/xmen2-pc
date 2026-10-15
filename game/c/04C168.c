/* Apocalypse_StateDrop - APOCALYPSE state: fall forward (vx $100 the way it faces, vy $300) with
 * its drop animation (Tbl_ApocalypseAnim3) and sound $E8; then Apocalypse_StateDropFall.
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_StateDrop(register Apocalypse *self)
{
	register short vx = 0x100;

	if (!(self->spr.flags & 0x10))
		vx = -vx;
	self->vx = vx;
	self->vy = 0x300;
	Obj_SetAnim(self, CODEPTR(Tbl_ApocalypseAnim3));
	Snd_Stop(0xE8);
	Snd_Play(0xE8);
	SET_STATE(self, (StateFn)Apocalypse_StateDropFall);
	return self->flags & 8;
}
