/* Hornet_StateAttackDelay - HORNET state between volleys (Hornet_Init's first state): face the
 * player and start the next volley (Hornet_StateFacePlayer) after attackDelay frames, or at once
 * when it is -1.
 *
 * self: the hornet. Returns self->flags & 8, or what Hornet_StateFacePlayer returns. */
#include "xmen2.h"

short Hornet_StateAttackDelay(Hornet *self)
{
	if (self->attackDelay == -1) {
		self->state = (StateFn)Hornet_StateFacePlayer;
		return Hornet_StateFacePlayer(self);
	}
	self->timer = self->attackDelay;
	self->state = (StateFn)Hornet_StateFacePlayer;
	return self->flags & 8;
}
