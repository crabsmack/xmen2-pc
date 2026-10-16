/* CapedAcolyte_ActAnimLow - CAPEDACOLYTE script action: the pose that shoots from lower down
 * (Tbl_CapedAcolyteAnim3: CapedAcolyte_StateShoot fires from 18 px ahead, 34 px up), then the next
 * script step on the next update.
 *
 * self: the object. */
#include "xmen2.h"

void CapedAcolyte_ActAnimLow(register CapedAcolyte *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_CapedAcolyteAnim3));
	AI_NEXT_STEP(self, self->ai, 0);
}
