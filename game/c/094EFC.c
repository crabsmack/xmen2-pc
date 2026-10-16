/* EvilDrop_StateStart - EVILDROP first state: EvilDrop_StateDrop from the next frame.
 *
 * self: the capsule. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short EvilDrop_StateStart(EvilDrop *self)
{
	SET_STATE(self, EvilDrop_StateDrop);
	return self->flags & 8;
}
