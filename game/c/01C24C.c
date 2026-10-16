/* YoyoAcolyte_OutsideActiveWindow - YOYOACOLYTE OutsideActiveWindow (class slot +$42): never
 * outside while the yoyo is out (so it is not deactivated mid-throw), else the parent's (BIONPC)
 * test.
 *
 * self: the object.
 * Returns 1 outside, 0 not. */
#include "xmen2.h"

long YoyoAcolyte_OutsideActiveWindow(register YoyoAcolyte *self)
{
	if (self->yoyoFlags & 1)
		return 0;
	return (*CLASS(Tbl_YoyoAcolyte_vtable)->parent->method[15])(self);	/* BIONPC OutsideActiveWindow */
}
