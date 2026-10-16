/* ExodusBlast_StateStart - EXODUSBLAST's first state: hidden (SetFrame $FF), then launch from the
 * next update (ExodusBlast_StateLaunch).
 *
 * self: the blast. Returns self->flags & 8. */
#include "xmen2.h"

short ExodusBlast_StateStart(register ExodusBlast *self)
{
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
	SET_STATE(self, ExodusBlast_StateLaunch);
	return self->flags & 8;
}
