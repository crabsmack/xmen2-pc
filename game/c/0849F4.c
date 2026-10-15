/* SmallBrood_ActWalk - SMALLBROOD AI-script action: walk the way it faces
 * (SmallBrood_StateApproach).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short SmallBrood_ActWalk(SmallBrood *self)
{
	if (self->spr.flags & 0x10)
		self->vx = -self->speed;
	else
		self->vx = self->speed;
	Obj_ChangeAnim(self, CODEPTR(Tbl_SmallBroodAnim));
	SET_STATE(self, SmallBrood_StateApproach);
	return self->flags & 8;
}
