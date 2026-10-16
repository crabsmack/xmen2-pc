/* YoyoAcolyte_Init - YOYOACOLYTE constructor (class slot +$12): the parent's (BIONPC) Init, then
 * an acolyte standing at its placement with its yoyo in hand.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry, or null
 * (then a default one at x, y: script 0, idle time 80).
 * No vertical speed or gravity; stats Tbl_YoyoAcolyteStats (health from them); anchor px/py =
 * the placement position (read even when pl is null: the 68000 then reads the vector table);
 * state (and default state) YoyoAcolyte_ActIdle100, hurt state YoyoAcolyte_StateDie; flags6 |=
 * $4082; sprite Art_YoyoAcolyteSpr, palette 1; no target, the yoyo cleared and not out.
 * Placement params: +$A the AI script (switch cases YoyoAcolyte_Init_Script0 /
 * _Script1to4, then YoyoAcolyte_Init_SetScript): 0 (and anything outside 0-4)
 * Tbl_YoyoAcolyte_ScriptLevel, 1 ..ScriptHighLen, 2 ..ScriptLowLen, 3 ..ScriptMixALen,
 * 4 ..ScriptMixBLen (thunks YoyoAcolyte_ScriptLevel, _ScriptHigh, _ScriptLow, _ScriptMixA,
 * _ScriptMixB: throws, each followed by ActIdle); +$C idleTime (0: 80).
 *
 * The AI script's jmp-thunk tables (separate in the disassembly): YoyoAcolyte_ScriptHigh, YoyoAcolyte_ScriptLow, YoyoAcolyte_ScriptMixA, YoyoAcolyte_ScriptMixB.
 */
#include "xmen2.h"

void YoyoAcolyte_Init(register YoyoAcolyte *self, register long x, register long y, register Placement *pl)
{
	Placement none;
	register short v;		/* the placement param being read */

	(*CLASS(Tbl_YoyoAcolyte_vtable)->parent->method[3])(self, x, y, pl);	/* BIONPC Init */
	self->vy = 0;
	self->ay = 0;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_YoyoAcolyteStats))->health;
#ifdef XMEN2_MPW
	self->px = pl->x;
	self->py = pl->y;
#else
	{
		/* no placement: the 68000 reads the position from the vector table */
		Placement *at = pl ? pl : (Placement *)Vectors;

		self->px = at->x;
		self->py = at->y;
	}
#endif
	self->state = YoyoAcolyte_ActIdle100;
	self->defaultState = YoyoAcolyte_ActIdle100;
	self->hurtState = (StateFn)YoyoAcolyte_StateDie;
	self->flags6 |= 0x4082;
	self->spr.def = CODEPTR(Art_YoyoAcolyteSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->target = 0;
	self->yoyoFlags = 0;
	{
		YoyoAcolyteYoyo *yo = &self->yoyo;

		yo->x = 0;
		yo->y = 0;
		yo->vx = 0;
		yo->vy = 0;
		yo->ax = 0;
		yo->ay = 0;
		yo->frame = 0;
		yo->frameTimer = 0;
	}
	if (!pl) {
		pl = &none;
		pl->x = x >> 8;
		pl->y = y >> 8;
		pl->param[0] = 0;
		pl->param[1] = 0;
		pl->param[2] = 0;
		pl->param[3] = 0x50;
		pl->param[4] = 0;
	}
	self->scriptNo = pl->param[2];
	{
		void *script;

		switch (v = pl->param[2]) {
		default:
		case 0:		/* YoyoAcolyte_Init_Script0 */
			script = CODEPTR(Tbl_YoyoAcolyte_ScriptLevel);
			break;
		case 1:		/* YoyoAcolyte_Init_Script1to4 */
			script = CODEPTR(Tbl_YoyoAcolyteScriptHigh);
			break;
		case 2:
			script = CODEPTR(Tbl_YoyoAcolyteScriptLow);
			break;
		case 3:
			script = CODEPTR(Tbl_YoyoAcolyteScriptMixA);
			break;
		case 4:
			script = CODEPTR(Tbl_YoyoAcolyte_ScriptMixB);
		}
		AiScript_Set(&self->ai, script);	/* YoyoAcolyte_Init_SetScript */
	}
	if ((v = pl->param[3]) != 0)
		self->idleTime = v;
	else
		self->idleTime = 0x50;
}
