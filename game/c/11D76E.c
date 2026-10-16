/* BabySentinelB_Init - BABYSENTINELB Init (class slot +$12): the parent's (BABYBIO) Init, then
 * palette 1, the stats Tbl_BabySentinelBStats (health from them), no frame shown ($FF, its slot
 * +$56), and its AI script from the first step: variant 1 Tbl_BabySentinelB_Script1 (thunks
 * BabySentinelB_Script1 / _Part2: ActPlayAnimD, ActFacePlayer, ActStopWait60, ActAttachFlame,
 * ActStopWaitParam, ActDropFlame, ActStopWait60 x2), else Tbl_BabySentinelB_Script2 (thunks
 * BabySentinelB_Script2 / _Part2: ActWaitPlayerNear192, ActFacePlayer, ActPlayAnimA,
 * ActPlayAnimD, ActStopWait60, ActAttachFlame, ActStopWaitParam, ActDropFlame, ActStopWait30,
 * ActPlayAnimA, ActPlayAnimB, ActStopWait60, ActStopWait30): a flame-carrying sentinel.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry or 0. */
#include "xmen2.h"

void BabySentinelB_Init(register BabySentinelB *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BabySentinelB_vtable)->parent->method[3])(self, x, y, pl);	/* BABYBIO Init */
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_BabySentinelBStats))->health;
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame */
	if (self->variant == 1)
		AiScript_Set(&self->ai, CODEPTR(Tbl_BabySentinelB_Script1));
	else
		AiScript_Set(&self->ai, CODEPTR(Tbl_BabySentinelB_Script2));
	AI_FIRST_STEP(self, self->ai, 0);
}
