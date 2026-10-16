/* BabyBio_ActPlayAnimB - BABYBIO AI-script action: show frame 0 (Tbl_BabyBioAnim), next step on the
 * next update.
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_ActPlayAnimB(BabyBio *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BabyBioAnim));
	AI_NEXT_STEP(self, self->ai, 0);
}
