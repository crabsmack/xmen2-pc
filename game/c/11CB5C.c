/* BabyBio_ActPlayAnimD - BABYBIO AI-script action: loop frames 8, 9 (Tbl_BabyBioAnim4), next
 * step now.
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_ActPlayAnimD(BabyBio *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BabyBioAnim4));
	AI_NEXT_STEP(self, self->ai, 0);
}
