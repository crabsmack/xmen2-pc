/* ChildShip_StateReleaseChild - CHILDSHIP state (crashed): a BRAINCHILD comes out of the wreck
 * (its New, slot +$0E, at the ship's position), flashing 20 frames (no hits) and facing the same
 * way (sprite flag $10); the ship blows up in 60 frames (ChildShip_StateDie).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short ChildShip_StateReleaseChild(register ChildShip *self)
{
	BrainChild *child;

	if ((child = (BrainChild *)(*CLASS(Tbl_BrainChild_vtable)->method[2])(0,
	    CLASS(Tbl_BrainChild_vtable), self->x, self->y, 0))) {	/* New */
		child->hitCooldown = 20;
		if (self->spr.flags & 0x10)
			child->spr.flags |= 0x10;
	}
	SET_STATE_WAIT(self, (StateFn)ChildShip_StateDie, 0x3C);
	return self->flags & 8;
}
