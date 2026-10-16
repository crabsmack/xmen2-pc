/* ChildShip_StateKnockback - CHILDSHIP state (after ChildShip_OnHurt): slide for up to 30 frames
 * (shake) from the next update (ChildShip_StateKnockbackSlow).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short ChildShip_StateKnockback(ChildShip *self)
{
	self->shake = 0x1E;
	SET_STATE(self, ChildShip_StateKnockbackSlow);
	return self->flags & 8;
}
