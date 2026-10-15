/* Brood_StateRetreatStart - BROOD state: walk away from where it faces (speed $1A0) for 50 frames
 * (Brood_StateRetreat).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short Brood_StateRetreatStart(Brood *self)
{
	short vx = -0x1A0;

	if (self->spr.flags & 0x10)
		vx = -vx;
	self->vx = vx;
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0F0));
	self->countdown = 50;
	SET_STATE(self, (StateFn)Brood_StateRetreat);
	return self->flags & 8;
}
