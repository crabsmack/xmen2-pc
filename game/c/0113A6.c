/* BigAcolyte_StateWaitYShoot - BIGACOLYTE state: check every 4 frames whether it has reached its
 * target height; then shoot (BigAcolyte_StateShootAnim).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short BigAcolyte_StateWaitYShoot(register BigAcolyte *self)
{
	if (self->arrivedY) {
		SET_STATE(self, BigAcolyte_StateShootAnim);
		return self->flags & 8;
	} else {
		SET_STATE_WAIT(self, BigAcolyte_StateWaitYShoot, 4);
		return self->flags & 8;
	}
}
