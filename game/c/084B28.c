/* SmallBrood_ActChase - SMALLBROOD AI-script action: face the nearest player and walk at it
 * (SmallBrood_StateChase).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short SmallBrood_ActChase(SmallBrood *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (p->x < self->x) {
		self->spr.flags |= 0x10;
		self->vx = -self->speed;
	} else {
		self->spr.flags &= ~0x10;
		self->vx = self->speed;
	}
	Obj_ChangeAnim(self, CODEPTR(Tbl_SmallBroodAnim));
	SET_STATE(self, SmallBrood_StateChase);
	return self->flags & 8;
}
