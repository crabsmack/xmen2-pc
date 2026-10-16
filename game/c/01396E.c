/* CapedAcolyte_ActStartMainScript - CAPEDACOLYTE intro script's last step: switch to the main AI
 * script (Tbl_CapedAcolyteMainScript) and start its first step on the next update.
 * Its 5 steps are the thunks CapedAcolyte_MainScript (ActAnimLow, ActHover, ActFacePlayer,
 * ActShootBurst) and CapedAcolyte_MainScriptEnd (ActFacePlayer), repeated.
 *
 * self: the object. */
#include "xmen2.h"

void CapedAcolyte_ActStartMainScript(register CapedAcolyte *self)
{
	AiScript_Set(&self->ai, CODEPTR(Tbl_CapedAcolyteMainScript));
	AI_FIRST_STEP(self, self->ai, 0);
}
