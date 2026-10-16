/* BrainChild_StateFall - BRAINCHILD state: falling; once faster than $180, frame 3 (slot +$56)
 * and fall to its height (BrainChild_StateFallHome).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short BrainChild_StateFall(BrainChild *self)
{
	if (self->vy > 0x180) {
		(*self->cls->method[20])(self, 3);	/* +$56 SetFrame */
		SET_STATE(self, BrainChild_StateFallHome);
		return self->flags & 8;
	}
	SET_STATE(self, BrainChild_StateFall);
	return self->flags & 8;
}
