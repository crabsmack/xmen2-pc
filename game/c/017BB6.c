/* FarAcolyte_StateAppear - FARACOLYTE state: show frame 0, then wait 60 frames for the player
 * (FarAcolyte_StateWaitPlayer).
 *
 * self: the acolyte. Returns self->flags & 8. */
#include "xmen2.h"

short FarAcolyte_StateAppear(register FarAcolyte *self)
{
	(*self->cls->method[20])(self, 0);		/* SetFrame */
	SET_STATE_WAIT(self, FarAcolyte_StateWaitPlayer, 60);
	return self->flags & 8;
}
