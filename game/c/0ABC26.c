/* FloatBubble_Deactivate - FLOATBUBBLE Deactivate (class slot +$46): the parent's (ACTOR), then
 * remove itself: a bubble that leaves the active window is gone for good.
 *
 * self: the bubble. */
#include "xmen2.h"

void FloatBubble_Deactivate(FloatBubble *self)
{
	(*CLASS(Tbl_FloatBubble_vtable)->parent->method[16])(self);	/* ACTOR Deactivate */
	(*self->cls->method[19])(self);		/* +$52 Kill */
}
