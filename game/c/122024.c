/* BigBio_ActIdleAnim90 - BIGBIO: idle animation, next step of the script in 90 frames (the state
 * BigBio_StateMorphToShape ends in).
 *
 * self: the boss. Never runs in the corpus. */
#include "xmen2.h"

void BigBio_ActIdleAnim90(register BigBio *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_1206E6));
	AI_NEXT_STEP(self, self->ai, 90);
}
