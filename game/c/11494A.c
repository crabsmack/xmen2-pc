/* XNpc_StateDeathFling - XNPC state (XNpc_DeathReact): fling the body (XNpc_StartDeathFling)
 * with the line it explodes below at its y, and fly (XNpc_StateDeathFlying, now).
 *
 * self: the object.
 * The line is kept in py. */
#include "xmen2.h"

void XNpc_StateDeathFling(XNpc *self)
{
	self->py = self->y >> 8;
	XNpc_StartDeathFling(self);
	self->state = XNpc_StateDeathFlying;
	XNpc_StateDeathFlying(self);
end:	;	/* (here MPW C pops all the arguments, as the ROM does) */
}
