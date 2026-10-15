/* Tuskette_StateHopBack - TUSKETTE hopper: face its home x (px, the pixel x it was placed at)
 * and jump that way (vx `speed`, vy -$7D0, gravity 100) with the hop animation; flags6 bit
 * $4000 off; then wait to fall (Tuskette_StateHopBackRise).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateHopBack(Tuskette *self)
{
	register long home;

	home = self->px;
	home <<= 8;
	if (home < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim3));
	self->vx = self->spr.flags & 0x10 ? -self->speed : self->speed;
	self->vy = -0x7D0;
	self->ay = 100;
	self->flags6 &= ~0x4000;
	SET_STATE(self, Tuskette_StateHopBackRise);
	return self->flags & 8;
}
