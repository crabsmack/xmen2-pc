/* Tuskette_StateHopOut - TUSKETTE hopper: jump the way it faces (vx `speed`, vy -$7D0, gravity
 * 100) with the hop animation; flags6 bit $1000 on (until Tuskette_StateHopDone), $4000 off;
 * then wait to fall (Tuskette_StateHopRise).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateHopOut(Tuskette *self)
{
	self->flags6 |= 0x1000;
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim3));
	self->vx = self->spr.flags & 0x10 ? -self->speed : self->speed;
	self->vy = -0x7D0;
	self->ay = 100;
	self->flags6 &= ~0x4000;
	SET_STATE(self, Tuskette_StateHopRise);
	return self->flags & 8;
}
