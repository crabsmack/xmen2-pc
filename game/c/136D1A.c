/* Hornet_StateDropIn - HORNET state of a DEATHBIRD hornet (Hornet_SpawnForDeathbird's first):
 * start falling into the arena (vy $400) and watch for the bottom (Hornet_StateDropping).
 *
 * self: the hornet. Returns self->flags & 8. */
#include "xmen2.h"

short Hornet_StateDropIn(Hornet *self)
{
	self->vy = 0x400;
	SET_STATE(self, (StateFn)Hornet_StateDropping);
	return self->flags & 8;
}
