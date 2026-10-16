/* BabyMoleMan_Init - BABYMOLEMAN Init (class slot +$12): the parent's (BABYBIO) Init, then
 * palette 1, stats Tbl_BabyMoleManStats (and their health), and the AI script its variant picks
 * (placement param 4), from the first step on its first update (no script: the default state):
 *   1 Tbl_BabyMoleManAiScript, 2 Tbl_BabyMoleMan_Script1 (BabyMoleMan_Script1: frames 5/6,
 *   walk, fire, wait 30), 3 Tbl_BabyMoleManAiScript2, 4 Tbl_BabyMoleManAiScript3, other
 *   Tbl_BabyMoleMan_ScriptDefault (BabyMoleMan_ScriptDefault: frames 5/6, wait for a player
 *   within 128 px, fire, wait 30).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on). */
#include "xmen2.h"

void BabyMoleMan_Init(BabyMoleMan *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BabyMoleMan_vtable)->parent->method[3])(self, x, y, pl);	/* BABYBIO Init */
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_BabyMoleManStats))->health;
	switch (self->variant) {
	case 1:		/* BabyMoleMan_Init_Case0 */
		AiScript_Set(&self->ai, CODEPTR(Tbl_BabyMoleManAiScript));
		break;
	case 2:		/* BabyMoleMan_Init_Case1 */
		AiScript_Set(&self->ai, CODEPTR(Tbl_BabyMoleMan_Script1));
		break;
	case 3:		/* BabyMoleMan_Init_Case2 */
		AiScript_Set(&self->ai, CODEPTR(Tbl_BabyMoleManAiScript2));
		break;
	case 4:
		AiScript_Set(&self->ai, CODEPTR(Tbl_BabyMoleManAiScript3));
		break;
	default:	/* BabyMoleMan_Init_Default */
		AiScript_Set(&self->ai, CODEPTR(Tbl_BabyMoleMan_ScriptDefault));
		break;
	}
	AI_FIRST_STEP(self, self->ai, 0);
}
