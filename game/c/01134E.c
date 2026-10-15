/* BigAcolyte_ActStartFight - BIGACOLYTE script action (last step of the intro, and of the fight
 * script, which so restarts): stop sound $34, switch to the fight script
 * (Tbl_BigAcolyteFightScriptLen: BigAcolyte_FightScript, 20 steps of the Rise/Go/Wait actions,
 * the last one this action again) and start it at its first step now.
 *
 * self: the boss. */
#include "xmen2.h"

void BigAcolyte_ActStartFight(register BigAcolyte *self)
{
	Snd_Stop(0x34);
	AiScript_Set(&self->ai, CODEPTR(Tbl_BigAcolyteFightScriptLen));
	AI_FIRST_STEP(self, self->ai, 0);
}
