/* BigBio_StartScript - BIGBIO: idle animation and start its AI script (BigBio_Script) from the
 * first step in 60 frames.
 *
 * self: the boss (after BigBio_StateWalkIn). */
#include "xmen2.h"

void BigBio_StartScript(register BigBio *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_1206E6));
	AiScript_Set(&self->ai, CODEPTR(Tbl_BigBio_Script));
	AI_FIRST_STEP(self, self->ai, 60);
}
