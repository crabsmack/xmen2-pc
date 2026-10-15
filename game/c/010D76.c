/* BigAcolyte_ActMoveToWall - BIGACOLYTE intro script action (step 3): glide to its place at the
 * wall (x $1B12, y $3D2) and wait there (BigAcolyte_StateWaitArrive).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short BigAcolyte_ActMoveToWall(register BigAcolyte *self)
{
	BigAcolyte_SetTargetX(self, 0x1B12);
	BigAcolyte_SetTargetY(self, 0x3D2);
	SET_STATE(self, BigAcolyte_StateWaitArrive);
	return self->flags & 8;
}
