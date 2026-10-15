/* Boulder_Kill - BOULDER Kill (class slot +$52): only the parent's (ACTOR).
 *
 * self: the boulder. */
#include "xmen2.h"

void Boulder_Kill(Boulder *self)
{
	(*CLASS(Tbl_Boulder_vtable)->parent->method[19])(self);	/* ACTOR Kill */
}
