/* Barbarian_StateEngage - BARBARIAN state: go for the target - flags6 bit $1000 on, face it;
 * walk up to it when it stands on BARBARIAN's own segment (Barbarian_StateApproach), else leap
 * across (Barbarian_StateTryLeap).
 *
 * self: the object (target and targetSeg set by Barbarian_TargetPlayer).
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateEngage(Barbarian *self)
{
	self->flags6 |= 0x1000;
	if (self->target->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	if (self->targetSeg == self->seg) {
		SET_STATE(self, Barbarian_StateApproach);
		return self->flags & 8;
	}
	SET_STATE(self, Barbarian_StateTryLeap);
	return self->flags & 8;
}
