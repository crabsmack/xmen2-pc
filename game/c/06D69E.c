/* Brood_ActIdle - BROOD AI-script action: stand still (idle animation), next step in 60 frames.
 *
 * self: the object. */
#include "xmen2.h"

void Brood_ActIdle(Brood *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0D8));
	STOP_MOVING(self);
	AI_NEXT_STEP(self, self->ai, 60);
}
