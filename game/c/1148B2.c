/* XNpc_StartDeathFling - set up an XNPC's death fling: thrown at $400 a frame away the way it was
 * knocked, up at |knock| * 4 (8 fraction bits), gravity $20, not hittable, inactive (flags6 |=
 * $5080).
 *
 * self: the object (its knock distance from XNpc_DeathReact). */
#include "xmen2.h"

void XNpc_StartDeathFling(XNpc *self)
{
	long d = (long)self->knock << 8;

	self->ay = 0x20;
	self->vx = 0x400;
	if (d < 0)
		self->vx = -self->vx;
	self->ax = 0;
	d = ABS(d);
	self->vy = (short)(-d >> 6);
	self->flags6 |= 0x5080;
}
