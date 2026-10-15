/* BigAcolyte_ActGoMid2 - BIGACOLYTE fight script action: go to the middle height (level 2, y $3B2)
 * without shooting, then the next step (BigAcolyte_StateWaitYNext).
 *
 * self: the boss (never runs in the corpus). Returns self->flags & 8. */
#include "xmen2.h"

short BigAcolyte_ActGoMid2(register BigAcolyte *self)
{
	self->level = 2;
	BigAcolyte_SetTargetY(self, 0x3B2);
	SET_STATE(self, BigAcolyte_StateWaitYNext);
	return self->flags & 8;
}
