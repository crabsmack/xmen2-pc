/* XNpc_KillOrExplode - an OnDeath (class slot +$72; SWAMPFLY, MAINTROBOT): hit types $D-$F just
 * remove the object (Kill, +$52), any other blows it up (Explode, +$7E).
 *
 * self: the object.  damage: unused.  type: hit type. */
#include "xmen2.h"

void XNpc_KillOrExplode(XNpc *self, char damage, char type)
{
	if (type == 13 || type == 14 || type == 15)
		(*self->cls->method[19])(self);	/* +$52 Kill */
	else
		(*self->cls->method[30])(self);	/* +$7E Explode */
}
