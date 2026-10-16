/* RollingDiscs_Move - ROLLINGDISCS Move (class slot +$4E): only the parent's (ACTOR) Move.
 *
 * self: the object.  steps: passed on. Never runs in the corpus. */
#include "xmen2.h"

void RollingDiscs_Move(RollingDiscs *self, short steps)
{
	(*CLASS(Tbl_RollingDiscs_vtable)->parent->method[18])(self, steps);	/* ACTOR Move */
}
