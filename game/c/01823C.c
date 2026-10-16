/* ShootAcolyte_ActWait30 - SHOOTACOLYTE script action (the last step, ShootAcolyte_ScriptEnd):
 * the next script step in 30 frames.
 *
 * self: the object. */
#include "xmen2.h"

void ShootAcolyte_ActWait30(register ShootAcolyte *self)
{
	AI_NEXT_STEP(self, self->ai, 30);
}
