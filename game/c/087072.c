/* BroodBoss_StateStompLand - BROODBOSS state: the stomp lands, animation off on frame 7, then the
 * floor shock (BroodBoss_StateShockOn).
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BroodBoss_StateStompLand(BroodBoss *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 7);		/* +$56 SetFrame */
	SET_STATE(self, BroodBoss_StateShockOn);
	return self->flags & 8;
}
