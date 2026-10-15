/* FarAcolyte_StateCastEnd - FARACOLYTE state: back to frame 0, then follow the bolt
 * (FarAcolyte_StateBoltFly) from the next frame.
 *
 * self: the acolyte. Returns self->flags & 8. */
#include "xmen2.h"

short FarAcolyte_StateCastEnd(register FarAcolyte *self)
{
	(*self->cls->method[20])(self, 0);		/* SetFrame */
	SET_STATE(self, FarAcolyte_StateBoltFly);
	return self->flags & 8;
}
