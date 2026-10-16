/* CapedAcolyte_ActAnimHigh - CAPEDACOLYTE script action: the pose that shoots from high up
 * (Tbl_CapedAcolyteAnim2: CapedAcolyte_StateShoot fires from 30 px ahead, 50 px up), then the next
 * script step on the next update.
 *
 * self: the object. */
#include "xmen2.h"

void CapedAcolyte_ActAnimHigh(register CapedAcolyte *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_CapedAcolyteAnim2));
	AI_NEXT_STEP(self, self->ai, 0);
}
