/* BroodBoss_StateVanish - BROODBOSS state: the body is gone (animation off, frame $FF: none); then
 * BroodBoss_StateClawOut in 30 frames.
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateVanish(BroodBoss *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame */
	SET_STATE_WAIT(self, BroodBoss_StateClawOut, 0x1E);
	return self->flags & 8;
}
