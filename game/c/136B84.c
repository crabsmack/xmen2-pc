/* Hornet_StateFacePlayer - HORNET state: turn toward the nearest player, load the volley's shot
 * count and fire it (Hornet_StateShotLoop, next frame).
 *
 * self: the hornet. Returns self->flags & 8. */
#include "xmen2.h"

short Hornet_StateFacePlayer(Hornet *self)
{
	XPlayer *p = Player_Nearest((Actor *)self);

	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	self->shotsLeft = self->volleyShots;
	SET_STATE(self, (StateFn)Hornet_StateShotLoop);
	return self->flags & 8;
}
