/* Ninja_ActStartCombat - NINJA script action (the intro script's second step,
 * Ninja_ScrStepStartCombat): switch to the combat script Tbl_NinjaCombatScript and run its first
 * step from the next update.
 *
 * self: the object. */
#include "xmen2.h"

void Ninja_ActStartCombat(register Ninja *self)
{
	AiScript_Set(&self->ai, CODEPTR(Tbl_NinjaCombatScript));
	AI_FIRST_STEP(self, self->ai, 0);
}
