/* Blast_StateLaunch - BLAST state: launch: the flight animation (Tbl_Anim_15B1E4), sound $D4
 * (restarted), x speed = speed; Blast_StateFly next frame.
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short Blast_StateLaunch(register Blast *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_15B1E4));
	Snd_Stop(0xD4);
	Snd_Play(0xD4);
	self->vx = self->speed;
	SET_STATE(self, Blast_StateFly);
	return self->flags & 8;
}
