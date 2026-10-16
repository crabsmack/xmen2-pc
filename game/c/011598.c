/* BigAcolyte_ActRise2 - BIGACOLYTE fight script action: go to the middle height (level 2, y $3B2)
 * and shoot there (BigAcolyte_StateWaitYShoot); already at level 2: shoot from the next update
 * (BigAcolyte_StateShootAnim).
 *
 * self: the boss (never runs in the corpus). Returns self->flags & 8. */
#include "xmen2.h"

short BigAcolyte_ActRise2(register BigAcolyte *self)
{
	if (self->level == 2) {
		SET_STATE(self, BigAcolyte_StateShootAnim);
		return self->flags & 8;
	}
	self->level = 2;
	BigAcolyte_SetTargetY(self, 0x3B2);
	SET_STATE(self, BigAcolyte_StateWaitYShoot);
	return self->flags & 8;
}
