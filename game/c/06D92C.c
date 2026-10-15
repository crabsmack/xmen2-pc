/* Brood_ActShoot - BROOD AI-script action: stop, face the nearest player and play the aiming
 * animation; it fires when the animation ends (Brood_StateFire).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short Brood_ActShoot(Brood *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	self->ax = 0;
	self->vx = 0;
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0FE));
	SET_STATE_WAIT(self, Brood_StateFire, 0xFF);
	return self->flags & 8;
}
