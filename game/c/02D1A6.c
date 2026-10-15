/* Exodus_AtkBlast17 - EXODUS attack: two blasts from 70 px above it, kind 1 (right) and kind 7
 * (left), with its casting animation (Tbl_ExodusAnim2) and sound $E0; then it becomes
 * hittable (Exodus_StateVulnerable).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Exodus_AtkBlast17(register Exodus *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_ExodusAnim2));
	ExodusBlast_Spawn(self->x, self->y - 0x4600, 1);
	ExodusBlast_Spawn(self->x, self->y - 0x4600, 7);
	Snd_Stop(0xE0);
	Snd_Play(0xE0);
	SET_STATE(self, Exodus_StateVulnerable);
	return self->flags & 8;
}
