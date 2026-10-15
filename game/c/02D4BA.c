/* Exodus_StateSwoopBlast - EXODUS state: at the bottom of the swoop fire the swoop blast (kind
 * swoopBlastKind) from 70 px above it, with its animation (Tbl_ExodusAnim6) and sound $E0; stop
 * moving, and rise again in 30 frames (Exodus_StateRiseStart).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Exodus_StateSwoopBlast(register Exodus *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_ExodusAnim6));
	ExodusBlast_Spawn(self->x, self->y - 0x4600, self->swoopBlastKind);
	Snd_Stop(0xE0);
	Snd_Play(0xE0);
	self->moveFlags = 0;
	SET_STATE_WAIT(self, Exodus_StateRiseStart, 30);
	return self->flags & 8;
}
