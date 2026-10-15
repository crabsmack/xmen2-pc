/* Exodus_AtkBlast2 - EXODUS attack: one blast of kind 2 (down-left) from 70 px above it, with
 * its animation (Tbl_ExodusAnim3) and sound $E0; then it becomes hittable
 * (Exodus_StateVulnerable).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Exodus_AtkBlast2(register Exodus *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_ExodusAnim3));
	ExodusBlast_Spawn(self->x, self->y - 0x4600, 2);
	Snd_Stop(0xE0);
	Snd_Play(0xE0);
	SET_STATE(self, Exodus_StateVulnerable);
	return self->flags & 8;
}
