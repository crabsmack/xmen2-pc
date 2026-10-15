/* BigAcolyte_ActGoTop - BIGACOLYTE fight script action: go to the top height (level 0, y $31A)
 * and shoot there (BigAcolyte_StateWaitYShoot).
 *
 * self: the boss (never runs in the corpus). Returns self->flags & 8. */
#include "xmen2.h"

short BigAcolyte_ActGoTop(register BigAcolyte *self)
{
	self->level = 0;
	BigAcolyte_SetTargetY(self, 0x31A);
	SET_STATE(self, BigAcolyte_StateWaitYShoot);
	return self->flags & 8;
}
