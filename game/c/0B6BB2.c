/* BrainChild_StateStart - BRAINCHILD's first state: leap from the next update
 * (BrainChild_StateLeap).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short BrainChild_StateStart(BrainChild *self)
{
	SET_STATE(self, BrainChild_StateLeap);
	return self->flags & 8;
}
