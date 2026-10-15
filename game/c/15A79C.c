/* Obj_OutsideActivateWindow - whether an object is outside its activation window: the opposite of
 * its InActivateWindow (class slot +$3A).
 *
 * self: the object. Returns 1 outside, 0 inside (a class slot override; Actor_Update's
 * OutsideActiveWindow form). */
#include "xmen2.h"

char Obj_OutsideActivateWindow(Actor *self)
{
	char in = (char)(*self->cls->method[13])(self);		/* +$3A InActivateWindow */

	return in ? 0 : 1;
}
