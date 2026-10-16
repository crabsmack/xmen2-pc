/* ChildShip_StateCrash2 - CHILDSHIP state: bouncing; at the floor again the screen shakes
 * (Tbl_ChildShipQuake) and it bounces higher (vy -$592, gravity $100), then
 * ChildShip_StateCrash3.
 *
 * self: the object. Returns self->flags & 8. */
#include "xmen2.h"

short ChildShip_StateCrash2(ChildShip *self)
{
	if (self->homeY >= 0x14000) {
		self->homeY = 0x14000;
		g_CamShake = CODEPTR(Tbl_ChildShipQuake);
		self->vy = -0x592;
		self->ay = 0x100;
		SET_STATE(self, ChildShip_StateCrash3);
		return self->flags & 8;
	}
	SET_STATE(self, ChildShip_StateCrash2);
	return self->flags & 8;
}
