/* EvilWolverine_ActLeap - EVILWOLVERINE AI-script action: leap the way it faces (off any platform:
 * Obj_Detach): vy -$300, gravity $3F, vx $396 (left when h-flipped, at most $396), sound $88 and
 * Wolverine's leap animation (Tbl_WolverineAnim); EvilWolverine_StateLeap from the next update.
 *
 * self: the clone (never runs in the corpus).
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short EvilWolverine_ActLeap(EvilWolverine *self)
{
	Obj_Detach(self);
	if (1) {	/* the ROM pops Obj_Detach's argument at the end of this block */
		self->vy = -0x300;
		self->ay = 0x3F;
		self->maxVx = 0x396;
		self->vx = self->spr.flags & 0x10 ? -0x396 : 0x396;
	}
	Snd_Stop(0x88);
	Snd_Play(0x88);
	Obj_SetAnim(self, CODEPTR(Tbl_WolverineAnim));
	SET_STATE(self, EvilWolverine_StateLeap);
	return self->flags & 8;
}
