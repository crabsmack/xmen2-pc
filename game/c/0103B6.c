/* BigAcolyte_OutsideActiveWindow - BIGACOLYTE class slot +$42: never outside while it is in the
 * fight (flags6 $1000) and alive; otherwise outside unless it is dying (BigAcolyte_DieEndScene).
 * No screen test at all.
 *
 * self: the boss. Returns 1 outside, 0 not. */
#include "xmen2.h"

char BigAcolyte_OutsideActiveWindow(register BigAcolyte *self)
{
	if ((self->flags6 & 0x1000) && self->health > 0)
		return 0;
	return !(self->state == (StateFn)BigAcolyte_DieEndScene);
}
