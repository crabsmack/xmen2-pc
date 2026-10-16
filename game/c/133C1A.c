/* DropRocket_StateDrop - DROPROCKET state while it falls: once its y is within 5 px of 64 px below
 * py (the pixel y it was made at: its Move doesn't update py), it stops falling and flies level
 * at $200 the way it faces (Tbl_BrainRocketAnim2), DropRocket_StateFly from the next update;
 * else it stays the state.
 *
 * self: the rocket. Returns flags & 8. */
#include "xmen2.h"

short DropRocket_StateDrop(DropRocket *self)
{
	register short y = self->y >> 8;

	if (ABS(y - (self->py + 0x40)) < 5) {
		Obj_SetAnim(self, CODEPTR(Tbl_BrainRocketAnim2));
		self->vy = 0;
		self->ay = 0;
		self->vx = !(self->spr.flags & 0x10) ? 0x200 : -0x200;
		SET_STATE(self, (StateFn)DropRocket_StateFly);
		return self->flags & 8;
	}
	SET_STATE(self, (StateFn)DropRocket_StateDrop);
	return self->flags & 8;
}
