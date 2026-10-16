/* BrainChild_ActShoot - BRAINCHILD script action: the shooting animation (Tbl_BrainChildAnim2),
 * firing when it ends (BrainChild_StateFire; timer $FF: the animation's end clears it).
 *
 * self: the object (a step of its AI script; never runs in the corpus). Returns
 * self->flags & 8. */
#include "xmen2.h"

short BrainChild_ActShoot(BrainChild *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_BrainChildAnim2));
	SET_STATE_WAIT(self, (StateFn)BrainChild_StateFire, 0xFF);
	return self->flags & 8;
}
