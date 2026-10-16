/* BrainChild_StateFallHome - BRAINCHILD state: falling; once down at py (the pixel y it was
 * placed at: BrainChild_Move doesn't update it), frame 3 (slot +$56) and land
 * (BrainChild_StateLand).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short BrainChild_StateFallHome(BrainChild *self)
{
	if (self->py << 8 <= self->y) {
		(*self->cls->method[20])(self, 3);	/* +$56 SetFrame */
		SET_STATE(self, (StateFn)BrainChild_StateLand);
		return self->flags & 8;
	}
	SET_STATE(self, BrainChild_StateFallHome);
	return self->flags & 8;
}
