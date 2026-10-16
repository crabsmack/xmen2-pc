/* RetractFloor_StateInit - RETRACTFLOOR first state (Init's): wait to be started
 * (RetractFloor_StateWait from the next update).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short RetractFloor_StateInit(register RetractFloor *self)
{
	SET_STATE(self, RetractFloor_StateWait);
	return self->flags & 8;
}
