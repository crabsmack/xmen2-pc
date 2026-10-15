/* Brood_StateBreakOut - BROOD state: run out of the corner the way it faces (speed $1A0) for 50
 * frames, still flashing (Brood_StateBreakOutRun).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short Brood_StateBreakOut(Brood *self)
{
	short vx = 0x1A0;

	if (self->spr.flags & 0x10)
		vx = -vx;
	self->vx = vx;
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0F0));
	self->countdown = 50;
	SET_STATE(self, (StateFn)Brood_StateBreakOutRun);
	return self->flags & 8;
}
