/* ShootAcolyte_ActWalk - SHOOTACOLYTE script action: frame 0, then walk towards the nearest player
 * (ShootAcolyte_StateWalkToPlayer from the next update) for at most walkTime frames.
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short ShootAcolyte_ActWalk(register ShootAcolyte *self)
{
	(*self->cls->method[20])(self, 0);		/* +$56 SetFrame */
	self->countdown = self->walkTime;
	SET_STATE(self, ShootAcolyte_StateWalkToPlayer);
	return self->flags & 8;
}
