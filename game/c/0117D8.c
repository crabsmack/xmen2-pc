/* BigAcolyte_ActGoLow1 - BIGACOLYTE fight script action: go to the low height (level 1, y $412)
 * without shooting, then the next step (BigAcolyte_StateWaitYNext).
 *
 * self: the boss (never runs in the corpus). Returns self->flags & 8. */
#include "xmen2.h"

short BigAcolyte_ActGoLow1(register BigAcolyte *self)
{
	self->level = 1;
	BigAcolyte_SetTargetY(self, 0x412);
	SET_STATE(self, BigAcolyte_StateWaitYNext);
	return self->flags & 8;
}
