/* ChildShip_StateCrash3 - CHILDSHIP state: bouncing; at the floor the last time the screen shakes
 * (Tbl_ChildShipQuake) and it stops, then it releases its child (ChildShip_StateReleaseChild).
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short ChildShip_StateCrash3(ChildShip *self)
{
	if (self->homeY >= 0x14000) {
		self->homeY = 0x14000;
		g_CamShake = CODEPTR(Tbl_ChildShipQuake);
		STOP_MOVING(self);
		SET_STATE(self, ChildShip_StateReleaseChild);
		return self->flags & 8;
	}
	SET_STATE(self, ChildShip_StateCrash3);
	return self->flags & 8;
}
