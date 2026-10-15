/* XNpc_StateDeathFlingHigh - XNPC state (XNpc_DeathReact, at an edge of its range): fling the
 * body (XNpc_StartDeathFling) with the line it explodes below 60 px under its y, and fly
 * (XNpc_StateDeathFlying, now). Never runs in the corpus.
 *
 * self: the object.
 * The line is kept in py. */
#include "xmen2.h"

void XNpc_StateDeathFlingHigh(XNpc *self)
{
	self->py = (self->y >> 8) + 60;
	XNpc_StartDeathFling(self);
	self->state = XNpc_StateDeathFlying;
	XNpc_StateDeathFlying(self);
end:	;	/* (here MPW C pops all the arguments, as the ROM does) */
}
