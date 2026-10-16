/* BabyBio_ActPlayAnimC - BABYBIO AI-script action: loop frames 5, 6 (Tbl_BabyBioAnim3), next
 * step on the next update.
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_ActPlayAnimC(BabyBio *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BabyBioAnim3));
	AI_NEXT_STEP(self, self->ai, 0);
}
