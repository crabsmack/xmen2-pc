/* Barbarian_StepTargetX - move BARBARIAN's targetX dx px further in the direction it faces,
 * but not past the end of segment seg.
 *
 * self: the object.  seg: the target's segment.  dx: pixels (20 from Barbarian_TargetPlayer). */
#include "xmen2.h"

void Barbarian_StepTargetX(Barbarian *self, Seg *seg, short dx)
{
	if (!(self->spr.flags & 0x10)) {
		self->targetX += dx;
		if (self->targetX > seg->x2)
			self->targetX = seg->x2;
	} else {
		self->targetX -= dx;
		if (self->targetX < seg->x1)
			self->targetX = seg->x1;
	}
}
