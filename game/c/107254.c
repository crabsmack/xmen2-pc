/* Platform1_StateStart - PLATFORM1's first state: take its own floor segment (g_Segs[segIndex],
 * on its draw layer) and start its mode: 0 shuttle (moving at its speed), 1 ridden, 2 switched
 * (the speed kept until the switch turns on).
 *
 * self: the platform. Returns flags & 8; nothing for other modes. */
#include "xmen2.h"

short Platform1_StateStart(register Platform1 *self)
{
	register Seg *seg = &g_Segs[self->segIndex];

	if (seg) {
		self->ownSeg = seg;
		seg->layer = self->layer;
	}
	switch (self->mode & 3) {
	case 0:
		self->vx = self->speed;
		SET_STATE(self, (StateFn)Platform1_StateShuttle);
		return self->flags & 8;
	case 1:
		SET_STATE(self, (StateFn)Platform1_StateRide);
		return self->flags & 8;
	case 2:
		self->savedSpeed = self->speed;
		SET_STATE(self, (StateFn)Platform1_StateSwitched);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
