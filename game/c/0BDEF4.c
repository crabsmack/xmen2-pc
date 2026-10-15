/* Barbarian_StateReachedTarget - BARBARIAN state: walked past targetX - no longer walking; then
 * by leapMode: 0 go for the target again (Barbarian_StateEngage), 1 try the leap again
 * (Barbarian_StateTryLeap2), else idle (Barbarian_StateIdle).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateReachedTarget(Barbarian *self)
{
	self->walking = 0;
	switch (self->leapMode) {
	case 0:
		SET_STATE(self, Barbarian_StateEngage);
		return self->flags & 8;
	case 1:
		SET_STATE(self, Barbarian_StateTryLeap2);
		return self->flags & 8;
	default:
		SET_STATE(self, Barbarian_StateIdle);
		return self->flags & 8;
	}
}
