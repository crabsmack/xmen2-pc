/* ChildShip_StateCrash1 - CHILDSHIP state: falling; at the floor (cruise y $140) the screen
 * shakes (g_CamShake = Tbl_ChildShipQuake) and it bounces (vy -$458, gravity $60), then
 * ChildShip_StateCrash2.
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short ChildShip_StateCrash1(ChildShip *self)
{
	if (self->homeY >= 0x14000) {
		self->homeY = 0x14000;
		g_CamShake = CODEPTR(Tbl_ChildShipQuake);
		self->vy = -0x458;
		self->ay = 0x60;
		SET_STATE(self, ChildShip_StateCrash2);
		return self->flags & 8;
	}
	SET_STATE(self, ChildShip_StateCrash1);
	return self->flags & 8;
}
