/* BioSentinel_StateFalling - BIOSENTINEL falling from the ceiling: once at its floor y, stop (no
 * motion, no animation), frame 6, stand on the floor segment, vulnerable, no longer kept active,
 * maximum speed its walking speed; BioSentinel_StateThink in 9 frames. Else keep falling.
 *
 * self: the sentinel. Returns self->flags & 8. */
#include "xmen2.h"

short BioSentinel_StateFalling(register BioSentinel *self)
{
	if (self->y >> 8 >= self->floorY) {
		STOP_MOVING(self);
		self->anim = 0;
		(*self->cls->method[20])(self, 6);		/* +$56 SetFrame */
		Obj_StandOnSeg(self, self->floorSeg);
		self->vulnerable = 1;
		self->flags6 &= ~0x1000;
		self->maxVx = self->walkSpeed;
		SET_STATE_WAIT(self, BioSentinel_StateThink, 9);
		return self->flags & 8;
	}
	SET_STATE(self, BioSentinel_StateFalling);
	return self->flags & 8;
}
