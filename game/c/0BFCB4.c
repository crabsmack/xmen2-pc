/* SwampBubbles_Deactivate - SWAMPBUBBLES Deactivate (class slot +$46): the parent's (ACTOR), then
 * it kills itself.
 *
 * self: the bubbles. Never runs in the corpus. */
#include "xmen2.h"

void SwampBubbles_Deactivate(SwampBubbles *self)
{
	(*CLASS(Tbl_SwampBubbles_vtable)->parent->method[16])(self);	/* ACTOR Deactivate */
	(*self->cls->method[19])(self);			/* +$52 Kill */
}
