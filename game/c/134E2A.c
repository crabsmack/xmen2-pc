/* FlyBrood_ActRestartScript - FLYBROOD's first state (set by FlyBrood_Init): its AI script from
 * the first step now (no script: the default state).
 *
 * self: the object. */
#include "xmen2.h"

void FlyBrood_ActRestartScript(FlyBrood *self)
{
	AI_FIRST_STEP(self, self->ai, 0);
}
