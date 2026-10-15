/* Gambit_StateAirCharge - GAMBIT state: A held in the air: he steers (class slot +$8A AirSteer);
 * once A is let go, the throw brakes the fall first (Gambit_StateAirThrowBrake, now). Landed:
 * the control state (slot +$A6), now.
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_StateAirCharge(register Gambit *self)
{
	if (!(self->flags & 0x1000))
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	else {
		(*self->cls->method[33])(self);			/* +$8A AirSteer */
		if (!(!self->pad ? g_Pad1Held & g_PadMaskA : g_Pad2Held & g_PadMaskA))
			GOTO_STATE(self, (StateFn)Gambit_StateAirThrowBrake);
	}
}
