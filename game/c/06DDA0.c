/* Brood_StateCornered - BROOD state, cornered after a hit: rage animation, flash for 20 frames
 * (Brood_StateCorneredFlash, palette toggled every 4 frames).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short Brood_StateCornered(Brood *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0E2));
	self->flashTimer = 4;
	self->countdown = 20;
	SET_STATE(self, Brood_StateCorneredFlash);
	return self->flags & 8;
}
