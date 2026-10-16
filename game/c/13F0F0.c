/* Pop_StateStart - POP state (Init's): start waiting (Pop_StateWait from the next update).
 *
 * self: the object.
 * Returns flags bit 3. Never runs in the corpus. */
#include "xmen2.h"

short Pop_StateStart(register Pop *self)
{
	SET_STATE(self, Pop_StateWait);
	return self->flags & 8;
}
