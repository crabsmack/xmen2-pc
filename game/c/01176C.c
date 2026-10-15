/* BigAcolyte_ActGoMidShoot - BIGACOLYTE fight script action: head for y $3B2 shooting on the way,
 * from 3 frames on (BigAcolyte_StateShootAnim; level 0 so the shots go on to the next step).
 *
 * self: the boss (never runs in the corpus). Returns self->flags & 8. */
#include "xmen2.h"

short BigAcolyte_ActGoMidShoot(register BigAcolyte *self)
{
	self->level = 0;
	BigAcolyte_SetTargetY(self, 0x3B2);
	SET_STATE_WAIT(self, BigAcolyte_StateShootAnim, 3);
	return self->flags & 8;
}
