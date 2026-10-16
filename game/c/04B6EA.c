/* Apocalypse_ActStartMainScript - APOCALYPSE script action (the last step of the entrance
 * scripts): switch to the bombing script (Tbl_Apocalypse_ScriptBomb: Apocalypse_ScriptBomb,
 * one step, Apocalypse_ActStartBombs) and start it now.
 *
 * self: the boss. */
#include "xmen2.h"

void Apocalypse_ActStartMainScript(register Apocalypse *self)
{
	AiScript_Set(&self->ai, CODEPTR(Tbl_Apocalypse_ScriptBomb));
	AI_FIRST_STEP(self, self->ai, 0);
}
