/* ChildShip_StateCrash - CHILDSHIP state (shot down, ChildShip_TakeDamage): smoke along
 * Tbl_ChildShipWreckTrail, fall (vy $100, gravity $100), then wait to hit the floor
 * (ChildShip_StateCrash1).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short ChildShip_StateCrash(ChildShip *self)
{
	self->trail = (PathPt *)CODEPTR(Tbl_ChildShipWreckTrail);
	self->vy = 0x100;
	self->ay = 0x100;
	SET_STATE(self, ChildShip_StateCrash1);
	return self->flags & 8;
}
