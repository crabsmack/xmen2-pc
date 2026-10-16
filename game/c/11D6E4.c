/* BabySentinelA_Init - BABYSENTINELA Init (class slot +$12): the parent's (BABYBIO) Init, then
 * palette 1, the stats Tbl_BabyMoleManStats (health from them) and its AI script (the table
 * Tbl_BabySentinelA_ScriptLen; thunks BabySentinelA_Script: ActCrawl, ActFacePlayer,
 * ActPlayAnimA, ActPlayAnimC, ActStopWait30, ActStartFiring, ActStopWait30, ActPlayAnimA). The
 * script starts at its next step change (the state stays BabyBio_StateStart).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry or 0. */
#include "xmen2.h"

void BabySentinelA_Init(register BabySentinelA *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BabySentinelA_vtable)->parent->method[3])(self, x, y, pl);	/* BABYBIO Init */
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_BabyMoleManStats))->health;
	AiScript_Set(&self->ai, CODEPTR(Tbl_BabySentinelA_ScriptLen));
}
