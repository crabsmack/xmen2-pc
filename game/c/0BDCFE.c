/* Barbarian_StateApproach - BARBARIAN state: on the target's segment - attack when it is within
 * $32 px (flags6 |= $4000, Barbarian_StateAttack), else walk to it (leapMode 0,
 * Barbarian_StateWalkStart). Marks it walking either way.
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateApproach(Barbarian *self)
{
	self->walking = 1;
	if (Barbarian_TargetClose(self)) {
		self->flags6 |= 0x4000;
		SET_STATE(self, Barbarian_StateAttack);
		return self->flags & 8;
	}
	self->leapMode = 0;
	SET_STATE(self, Barbarian_StateWalkStart);
	return self->flags & 8;
}
