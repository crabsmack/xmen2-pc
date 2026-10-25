/* CapedAcolyte_Init - CAPEDACOLYTE constructor (class slot +$12): the parent's (BIONPC) Init, then
 * an acolyte hovering around its placement.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry.
 * Home = position = the placement position; stats Tbl_CapedAcolyteStats (health from them);
 * sprite Art_CapedAcolyteSpr, palette 1; AI script Tbl_CapedAcolyteIntroScript from its
 * first step (thunks CapedAcolyte_IntroScript: ActAnimHigh, ActFacePlayer,
 * ActStartMainScript); flags6 |= $80; sprite priority (bit 2) copied from the live player (player 1 when
 * neither is); not moving; animation Tbl_CapedAcolyteAnim; hover table
 * Tbl_CapedAcolyteHoverOffsets. Placement params: +6 half width of the x range (|v|, 0: $40),
 * +8 height of the y range above the placement (|v|, 0: $60), +$A hoverTime (0: 120), +$C
 * burstShots (0: 2), +$E shotDelay (0: 20). */
#include "xmen2.h"

void CapedAcolyte_Init(register CapedAcolyte *self, long x, long y, register Placement *pl)
{
	register short v;		/* the placement param being read */

	(*CLASS(Tbl_CapedAcolyte_vtable)->parent->method[3])(self, x, y, pl);	/* BIONPC Init */
	self->homeX = (long)pl->x << 8;
	self->homeY = (long)pl->y << 8;
	self->x = self->homeX;
	self->y = self->homeY;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_CapedAcolyteStats))->health;
	self->spr.def = CODEPTR(Art_CapedAcolyteSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	AiScript_Set(&self->ai, CODEPTR(Tbl_CapedAcolyteIntroScript));
	AI_FIRST_STEP(self, self->ai, 0);
	self->flags6 |= 0x80;
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
	STOP_MOVING(self);
	Obj_SetAnim(self, CODEPTR(Tbl_CapedAcolyteAnim));
	self->hover = (HoverStep *)CODEPTR(Tbl_CapedAcolyteHoverOffsets);
	if ((v = pl->param[0]) != 0) {
		self->xMin = pl->x - ABS(v);
		self->xMax = pl->x + ABS(v);
	} else {
		self->xMin = pl->x - 0x40;
		self->xMax = pl->x + 0x40;
	}
	self->yMax = pl->y;
	if ((v = pl->param[1]) != 0)
		self->yMin = self->yMax - ABS(v);
	else
		self->yMin = self->yMax - 0x60;
	if ((v = pl->param[2]) != 0)
		self->hoverTime = v;
	else
		self->hoverTime = 0x78;
	if ((v = pl->param[3]) != 0)
		self->burstShots = v;
	else
		self->burstShots = 2;
	if ((v = pl->param[4]) != 0)
		self->shotDelay = v;
	else
		self->shotDelay = 0x14;
}
