/* XNpc_StateExplode - XNPC state (XNpc_DeathReact, hit types 11 and 12): blow up now (Explode,
 * class slot +$7E).
 *
 * self: the object. */
#include "xmen2.h"

void XNpc_StateExplode(XNpc *self)
{
	(*self->cls->method[30])(self);	/* +$7E Explode */
}
