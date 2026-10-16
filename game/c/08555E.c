/* Teleporter_StateStart - TELEPORTER first state: Teleporter_StateWait from the next frame.
 *
 * self: the pad. Returns flags & 8. */
#include "xmen2.h"

short Teleporter_StateStart(Teleporter *self)
{
	SET_STATE(self, Teleporter_StateWait);
	return self->flags & 8;
}
