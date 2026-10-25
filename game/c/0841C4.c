/* SmallBrood_Init - SMALLBROOD Init (class slot +$12): the parent's (BIONPC) Init, then the
 * sprite, stats, patrol range, speed, cooldown and AI script from the placement entry.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry.
 * Placement params: +6 / +8 extent left / right of x (0: $60; negative: that side is an open
 * edge it falls off, wallLeft/wallRight 0), +$A speed in pixels (0: 1), +$C cooldown (0: 90),
 * +$E variant |0-3| (0 walker, 1 kamikaze that first waits for a player and runs left unless
 * the param is negative, 2 approach, 3 chase; other values as 0). Variant 0 stands on its
 * spawn y (floor = y, ceiling = y - 1); the others find the floor below and the ceiling above
 * (none above: floor - $A0). The sprite priority bit (2) is copied from the live player (player 1
 * when neither is). Starts at SmallBrood_StateStart (variant 1: SmallBrood_StateWaitForPlayer),
 * keeps 80 pixels from the player, frame $FF.
 *
 * The AI script's jmp-thunk tables (separate in the disassembly): SmallBrood_ScrStepKamikaze, SmallBrood_ScrStepApproach, SmallBrood_ScrStepWalk.
 */
#include "xmen2.h"

void SmallBrood_Init(SmallBrood *self, long x, long y, Placement *pl)
{
	register short v;		/* the placement param being read */
	short d;			/* an extent */
	register char fwd;		/* variant param >= 0 */
	register short fy;		/* the floor y of variant 0 */

	(*CLASS(Tbl_SmallBrood_vtable)->parent->method[3])(self, x, y, pl);
	self->vy = 0;
	self->ay = 0;
	self->state = SmallBrood_StateStart;
	self->defaultState = SmallBrood_StateStart;
	self->flags6 |= 0x80;
	self->spr.def = CODEPTR(Art_SmallBroodSpr);
	self->spr.flags = self->spr.flags & ~3 | 3;
	if (IS_ALIVE(&g_Player1)) {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else if (IS_ALIVE(&g_Player2)) {
		if (g_Player2.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else
#if defined(XMEN2_NATIVE)
	if (!P34_CopyLiveLayer(&self->spr, 1))	/* players 1-2 out: a live player 3-4's (R11) */
#endif
	{
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	}
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_SmallBroodStats))->health;
	self->target = 0;
	self->aiFlags = 0;
	fwd = 1;
	d = (v = pl->param[0]) ? ABS(v) : 0x60;
	self->wallLeft = v ? (v > 0 ? 1 : 0) : 1;
	self->xMin = pl->x - d;
	d = (v = pl->param[1]) ? ABS(v) : 0x60;
	self->wallRight = v ? (v > 0 ? 1 : 0) : 1;
	self->xMax = pl->x + d;
	self->speed = (v = pl->param[2]) ? ABS(v) << 8 : 0x100;
	self->cooldownTime = (v = pl->param[3]) ? v : 90;
	self->cooldown = self->cooldownTime;
	if ((v = pl->param[4]) < 0) {
		fwd = 0;
		v = -v;
	}
	switch (v) {
	default:
	case 0:				/* SmallBrood_Init_CaseDefault */
		AiScript_Set(&self->ai, CODEPTR(Tbl_SmallBroodAiScript));
		break;
	case 1:				/* SmallBrood_Init_Case1 */
		self->state = (StateFn)SmallBrood_StateWaitForPlayer;
		self->defaultState = (StateFn)SmallBrood_StateWaitForPlayer;
		AiScript_Set(&self->ai, CODEPTR(Tbl_SmallBrood_ScrStepKamikazeLen));
		if (fwd)
			self->aiFlags |= 1;
		break;
	case 2:
		AiScript_Set(&self->ai, CODEPTR(Tbl_SmallBrood_ScrStepApproachLen));
		break;
	case 3:
		AiScript_Set(&self->ai, CODEPTR(Tbl_SmallBroodChaseScript));
		break;
	}
	/* SmallBrood_Init_SetYLimits */
	if (pl->param[4]) {
		register short px = pl->x;
		short py = pl->y - 32;
		Seg *seg;

		seg = Seg_FindBelow(px, py, 1000);
		if (seg)
			self->floorY = seg->y1 < seg->y2 ? seg->y1 : seg->y2;
		else
			self->floorY = pl->y;
		seg = Seg_FindAbove(px, py, 1000);
		if (seg)
			self->ceilY = seg->y1 > seg->y2 ? seg->y1 : seg->y2;
		else
			self->ceilY = self->floorY - 0xA0;
	} else {
		fy = pl->y;
		self->floorY = fy;
		self->ceilY = fy - 1;
	}
	self->keepDist = 0x50;
	(*self->cls->method[20])(self, 0xFF);	/* +$56 SetFrame */
}
