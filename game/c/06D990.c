/* Brood_StateFire - BROOD state: fire a shot from (10, -48) and play the recoil animation; when it
 * ends, Brood_StateFireEnd.
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short Brood_StateFire(Brood *self)
{
	(*self->cls->method[31])(self, 10, -0x30, 0);	/* +$82 Shoot: SmallBrood_Shoot */
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D10E));
	SET_STATE_WAIT(self, (StateFn)Brood_StateFireEnd, 0xFF);
	return self->flags & 8;
}
