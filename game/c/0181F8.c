/* ShootAcolyte_ActRestartScript - SHOOTACOLYTE: (re)start its AI script at step 0, run 30 frames
 * from now (the state Init gives it, and its default state).
 *
 * self: the object. */
#include "xmen2.h"

void ShootAcolyte_ActRestartScript(register ShootAcolyte *self)
{
	AI_FIRST_STEP(self, self->ai, 30);
}
