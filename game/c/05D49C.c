/* Tuskette_StateHopPause - TUSKETTE hopper, landed after the hop out: idle animation for 25
 * frames, then hop back (Tuskette_StateHopBack).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateHopPause(Tuskette *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim));
	SET_STATE_WAIT(self, Tuskette_StateHopBack, 25);
	return self->flags & 8;
}
