/* FarAcolyte_StateCast - FARACOLYTE state: casting frame 1, launch the bolt at the orb
 * (FarAcolyte_StartBolt), back to frame 0 in 4 frames (FarAcolyte_StateCastEnd).
 *
 * self: the acolyte. Returns self->flags & 8. */
#include "xmen2.h"

short FarAcolyte_StateCast(register FarAcolyte *self)
{
	(*self->cls->method[20])(self, 1);		/* SetFrame */
	(*self->cls->method[28])(self);			/* +$76 FarAcolyte_StartBolt */
	SET_STATE_WAIT(self, FarAcolyte_StateCastEnd, 4);
	return self->flags & 8;
}
