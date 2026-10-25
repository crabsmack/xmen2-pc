/* Brood_Init - BROOD Init (class slot +$12): the parent's (SMALLBROOD) Init, then BROOD's own
 * sprite, patrol range, speed, cooldown, floor, stats and AI script.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry.
 * Placement params as SMALLBROOD's (+6 / +8 extents, default $80 here; +$A speed; +$C
 * cooldown); +$E is read and not used (left over from SmallBrood_Init's variant switch). The
 * floor is the segment below the spawn point (none: spawn y), the ceiling 32 above it. Keeps
 * 120 pixels from the player. AiScript_Set runs twice: Tbl_BroodAiScript, then
 * Tbl_Brood_Script (charge, shoot) which is the one kept.
 *
 * The AI script's jmp-thunk tables (separate in the disassembly): Brood_Script.
 */
#include "xmen2.h"

void Brood_Init(Brood *self, long x, long y, Placement *pl)
{
	short v, d;

	(*CLASS(Tbl_Brood_vtable)->parent->method[3])(self, x, y, pl);
	self->vy = 0;
	self->ay = 0;
	self->state = Brood_StateStart;
	self->defaultState = Brood_StateStart;
	self->flags6 |= 0x80;
	self->spr.def = CODEPTR(Art_BroodSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
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
	self->target = 0;
	self->aiFlags = 0;
	d = (v = pl->param[0]) ? ABS(v) : 0x80;
	self->wallLeft = v ? (v > 0 ? 1 : 0) : 1;
	self->xMin = pl->x - d;
	d = (v = pl->param[1]) ? ABS(v) : 0x80;
	self->wallRight = v ? (v > 0 ? 1 : 0) : 1;
	self->xMax = pl->x + d;
	self->speed = (v = pl->param[2]) ? ABS(v) << 8 : 0x100;
	self->cooldownTime = (v = pl->param[3]) ? v : 90;
	self->cooldown = self->cooldownTime;
	v = pl->param[4];			/* unused */
	{
		short px = pl->x;
		short py = pl->y - 32;
		Seg *seg;

		seg = Seg_FindBelow(px, py, 1000);
		if (seg)
			self->floorY = seg->y1 < seg->y2 ? seg->y1 : seg->y2;
		else
			self->floorY = pl->y;
	}
	self->ceilY = self->floorY - 32;
	self->keepDist = 0x78;
	(*self->cls->method[20])(self, 0xFF);	/* +$56 SetFrame */
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_BroodStats))->health;
	AiScript_Set(&self->ai, CODEPTR(Tbl_BroodAiScript));
	AiScript_Set(&self->ai, CODEPTR(Tbl_Brood_Script));
	self->flashTimer = 0;
}
