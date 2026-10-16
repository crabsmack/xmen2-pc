/* Apocalypse_ActPickScript - APOCALYPSE script action: switch to script choice 0-3
 * (Tbl_ApocalypseChoiceScript0-3; 0 for any other value) and start it on the next update.
 *
 * self: the boss (never runs in the corpus).
 * The cases are Apocalypse_ActPickScript_part1 (0 and default), _part2 (1-3) and _part3 (the
 * script start). */
#include "xmen2.h"

void Apocalypse_ActPickScript(register Apocalypse *self)
{
	switch (self->choice) {
	default:
	case 0:
		AiScript_Set(&self->ai, CODEPTR(Tbl_ApocalypseChoiceScript0));
		break;
	case 1:
		AiScript_Set(&self->ai, CODEPTR(Tbl_ApocalypseChoiceScript1));
		break;
	case 2:
		AiScript_Set(&self->ai, CODEPTR(Tbl_ApocalypseChoiceScript2));
		break;
	case 3:
		AiScript_Set(&self->ai, CODEPTR(Tbl_ApocalypseChoiceScript3));
		break;
	}
	AI_FIRST_STEP(self, self->ai, 0);
}
