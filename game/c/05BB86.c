/* Boulder_Deactivate - BOULDER Deactivate (class slot +$46): only the parent's (ACTOR).
 *
 * self: the boulder. */
#include "xmen2.h"

void Boulder_Deactivate(Boulder *self)
{
	(*CLASS(Tbl_Boulder_vtable)->parent->method[16])(self);	/* ACTOR Deactivate */
}
