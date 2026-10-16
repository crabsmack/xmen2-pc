/* RetractFloor_StateKill - RETRACTFLOOR last state: kill itself (class slot +$52 Kill).
 *
 * self: the object. */
#include "xmen2.h"

void RetractFloor_StateKill(register RetractFloor *self)
{
	(*self->cls->method[19])(self);		/* +$52 Kill */
}
