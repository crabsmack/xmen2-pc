/* Tuskette_StateHopDone - TUSKETTE hopper, back home after a hop: flags6 bit $1000 off, idle
 * again (Tuskette_StateHopIdle).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateHopDone(Tuskette *self)
{
	self->flags6 &= ~0x1000;
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim));
	SET_STATE(self, Tuskette_StateHopIdle);
	return self->flags & 8;
}
