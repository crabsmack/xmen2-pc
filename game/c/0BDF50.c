/* Barbarian_StateTryLeap - BARBARIAN state: the target is on another segment - flags6 bit $4000
 * on and leap to it when it is in range (Barbarian_SetupLeap, Barbarian_StateLeap), else after 30
 * frames walk towards it instead (Barbarian_StateLeapFailed).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateTryLeap(Barbarian *self)
{
	self->flags6 |= 0x4000;
	if (Barbarian_SetupLeap(self)) {
		SET_STATE(self, Barbarian_StateLeap);
		return self->flags & 8;
	}
	SET_STATE_WAIT(self, Barbarian_StateLeapFailed, 30);
	return self->flags & 8;
}
