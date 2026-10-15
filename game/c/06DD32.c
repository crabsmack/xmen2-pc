/* Brood_StateRetreat - BROOD state: when the retreat countdown runs out, stop (idle animation)
 * and go on with the next AI-script step.
 *
 * self: the object. */
#include "xmen2.h"

void Brood_StateRetreat(Brood *self)
{
	if (self->countdown == 0) {
		self->ax = 0;
		self->vx = 0;
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0D8));
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
