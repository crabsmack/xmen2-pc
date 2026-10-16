/* BabySentinelC_Init - BABYSENTINELC Init (class slot +$12): the parent's (BABYBIO) Init, then
 * palette 1, stats Tbl_BabySentinelCStats (and their health), no frame shown ($FF, slot +$56),
 * and the AI script its variant picks (placement param 4): 1 Tbl_BabySentinelCAiScript, else
 * Tbl_BabySentinelC_Script2 (BabySentinelC_Script2: crawl, face the player, attach and drop
 * a flame...), from the first step on its first update (no script: the default state).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on). */
#include "xmen2.h"

void BabySentinelC_Init(BabySentinelC *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BabySentinelC_vtable)->parent->method[3])(self, x, y, pl);	/* BABYBIO Init */
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_BabySentinelCStats))->health;
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
	if (self->variant == 1)
		AiScript_Set(&self->ai, CODEPTR(Tbl_BabySentinelCAiScript));
	else
		AiScript_Set(&self->ai, CODEPTR(Tbl_BabySentinelC_Script2));
	AI_FIRST_STEP(self, self->ai, 0);
}
