/* Brood_StateFireEnd - BROOD state after a shot: idle animation, next AI-script step in 60
 * frames.
 *
 * self: the object. */
#include "xmen2.h"

void Brood_StateFireEnd(Brood *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0D8));
	AI_NEXT_STEP(self, self->ai, 60);
}
