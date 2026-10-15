/* FlyBrood_StateFacePlayer - FLYBROOD state after FlyBrood_ActHoverForever: keep facing the
 * nearest player.
 *
 * self: the object. */
#include "xmen2.h"

void FlyBrood_StateFacePlayer(FlyBrood *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
}
