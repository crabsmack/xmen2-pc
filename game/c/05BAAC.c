/* Boulder_StateShatter - a BOULDER is hit (its hurt state, Boulder_Init): it breaks into its three
 * pieces (Boulder_SplitIntoPieces), becomes unhittable (flags6 $1000), stops, drops its
 * animation, shows frame 4, stays "shattered" for 1000 frames (hitCooldown: Boulder_Move and
 * _Draw then work on the pieces), and lets them fall (Boulder_StatePiecesFall).
 *
 * self: the boulder. Returns self->flags & 8. */
#include "xmen2.h"

short Boulder_StateShatter(register Boulder *self)
{
	Boulder_SplitIntoPieces(self);
	self->flags6 |= 0x1000;
	STOP_MOVING(self);
	self->anim = 0;
	(*self->cls->method[20])(self, 4);		/* +$56 SetFrame */
	self->hitCooldown = 1000;
	SET_STATE(self, Boulder_StatePiecesFall);
	return self->flags & 8;
}
