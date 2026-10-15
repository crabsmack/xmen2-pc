/* XNpc_ShakeStep - one step of the hurt shake: x 1 px right when the shake counter's low two
 * bits are 2, 1 px left when they are 0 (so it shakes around where it was).
 *
 * self: the object. */
#include "xmen2.h"

void XNpc_ShakeStep(XNpc *self)
{
	short phase = self->shake & 3;

	if (phase == 2)
		self->x += 0x100;
	else if (phase == 0)
		self->x -= 0x100;
}
