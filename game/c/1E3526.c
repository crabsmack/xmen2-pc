/* Cyclops_StateAirCharge - CYCLOPS holding A in the air: steer (AirSteer, slot +$8A); A released:
 * brake and fire (Cyclops_StateAirBeamBrake, now).
 *
 * self: the player.
 * Landing first: the control state (slot +$A6), now. */
#include "xmen2.h"

void Cyclops_StateAirCharge(register Cyclops *self)
{
	if (!(self->flags & 0x1000))
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	else {
		(*self->cls->method[33])(self);			/* AirSteer */
		if (!(!self->pad ? g_Pad1Held & g_PadMaskA : g_Pad2Held & g_PadMaskA))
			GOTO_STATE(self, (StateFn)Cyclops_StateAirBeamBrake);
	}
}
