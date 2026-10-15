/* Exodus_AtkBlast4 - EXODUS attack: one blast of kind 4 (straight down) from 70 px above it,
 * with its animation (Tbl_ExodusAnim5) and sound $E0; then it becomes hittable
 * (Exodus_StateVulnerable).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Exodus_AtkBlast4(register Exodus *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_ExodusAnim5));
	ExodusBlast_Spawn(self->x, self->y - 0x4600, 4);
	Snd_Stop(0xE0);
	Snd_Play(0xE0);
	SET_STATE(self, Exodus_StateVulnerable);
	return self->flags & 8;
}
