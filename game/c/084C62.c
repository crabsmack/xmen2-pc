/* SmallBrood_ActShoot - SMALLBROOD AI-script action: stop, face the nearest player and play the
 * aiming animation; it fires when the animation ends (SmallBrood_StateFire).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short SmallBrood_ActShoot(SmallBrood *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	self->ax = 0;
	self->vx = 0;
	Obj_ChangeAnim(self, CODEPTR(Tbl_SmallBroodAnim2));
	SET_STATE_WAIT(self, SmallBrood_StateFire, 0xFF);
	return self->flags & 8;
}
