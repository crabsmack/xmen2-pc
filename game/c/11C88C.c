/* BabyBio_ActPlayAnimA - BABYBIO AI-script action: drop its flame, stop, show frame 4
 * (Tbl_BabyBioAnim2), next step in 8 frames.
 *
 * self: the object. */
#include "xmen2.h"

void BabyBio_ActPlayAnimA(BabyBio *self)
{
	if (self->flame) {
		(*self->flame->cls->method[19])(self->flame);	/* +$52 Kill */
		self->flame = 0;
	}
	STOP_MOVING(self);
	Obj_SetAnim(self, CODEPTR(Tbl_BabyBioAnim2));
	AI_NEXT_STEP(self, self->ai, 8);
}
