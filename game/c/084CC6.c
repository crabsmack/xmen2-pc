/* SmallBrood_StateFire - SMALLBROOD state: fire a shot from (18, -28) and play the recoil
 * animation; when it ends, the next AI-script step (SmallBrood_StateNextStep).
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short SmallBrood_StateFire(SmallBrood *self)
{
	(*self->cls->method[31])(self, 0x12, -0x1C, 0);	/* +$82 Shoot: SmallBrood_Shoot */
	Obj_ChangeAnim(self, CODEPTR(Tbl_SmallBroodAnim3));
	SET_STATE_WAIT(self, (StateFn)SmallBrood_StateNextStep, 0xFF);
	return self->flags & 8;
}
