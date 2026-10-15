/* SmallBrood_StateWaitCooldown - SMALLBROOD state: stand (idle animation) until the cooldown runs
 * out, then reload it and go on with the next AI-script step.
 *
 * self: the object. The cooldown counts down in SmallBrood_Update. */
#include "xmen2.h"

void SmallBrood_StateWaitCooldown(SmallBrood *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_SmallBroodAnim7));
	if (self->cooldown == 0) {
		self->cooldown = self->cooldownTime;
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
