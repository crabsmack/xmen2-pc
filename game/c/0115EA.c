/* BigAcolyte_ActRise1 - BIGACOLYTE fight script action: go to the low height (level 1, y $412)
 * and shoot there (BigAcolyte_StateWaitYShoot); already at level 1: shoot from the next update
 * (BigAcolyte_StateShootAnim).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short BigAcolyte_ActRise1(register BigAcolyte *self)
{
	if (self->level == 1) {
		SET_STATE(self, BigAcolyte_StateShootAnim);
		return self->flags & 8;
	}
	self->level = 1;
	BigAcolyte_SetTargetY(self, 0x412);
	SET_STATE(self, BigAcolyte_StateWaitYShoot);
	return self->flags & 8;
}
