/* Tuskette_StateHopIdle - TUSKETTE hopper: idle animation, then wait for a player to come near
 * (Tuskette_StateHopWait).
 *
 * self: the object (also where it comes back to after each hop, Tuskette_StateHopDone).
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateHopIdle(Tuskette *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim));
	SET_STATE(self, Tuskette_StateHopWait);
	return self->flags & 8;
}
