/* XNpc_Move - XNPC Move (class slot +$4E): the parent's (ACTOR) Move, nothing else.
 *
 * self:  the object.
 * steps: frame steps (1, or 2 in two-player step mode), passed on. */
#include "xmen2.h"

void XNpc_Move(XNpc *self, short steps)
{
	(*CLASS(Tbl_XNpc_vtable)->parent->method[18])(self, steps);	/* ACTOR Move */
}
