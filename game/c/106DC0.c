/* Platform1_Move - PLATFORM1's Move (class slot +$4E): no velocity or acceleration, only steps
 * along its floor-segment path at its speed (Seg_MoveAlong, once per frame step). At the path's
 * end (flags bit $100, set by Seg_MoveAlong) it stops: the speed kept in savedSpeed, 10 frames of
 * countdown (the states turn it round after them).
 *
 * self: the platform.  steps: frame steps (1, or 2 when the game runs at half rate). */
#include "xmen2.h"

void Platform1_Move(register Platform1 *self, register short steps)
{
	if (self->vx) {
		while (steps--)
			Seg_MoveAlong(self, self->vx);
		if (self->flags & 0x100) {
			self->savedSpeed = self->vx;
			self->vx = 0;
			self->countdown = 10;
		}
	}
}
