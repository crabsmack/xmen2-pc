/* Ninja_Init - NINJA constructor (class slot +$12): the parent's (BIONPC) Init, then a ninja
 * standing still at its placement, starting its intro script (Ninja_StateNextStep, also its
 * default state).
 *
 * self: the new object.  x, y: position (8 fraction bits; the pixel position is the placement's).
 * pl: its placement entry (never null).
 * Stats Tbl_NinjaStats, but 1 health; sprite set g_NinjaSprDef2 (or, when that is -1,
 * Art_NinjaSpr2 itself, palette 1); AI script Tbl_NinjaIntroScript; flags6 |= $80; sprite priority
 * (bit 2) as the live player's (player 1's when neither is alive); standing on the nearest floor
 * segment of type $3E9 (Seg_FindNearestOfType), detached, not moving, the idle animation
 * Tbl_NinjaAnimIdle. Placement params: 0 the patrol half-width (0: 160), 3 waitTime (0: 90),
 * 4 leaveCount (0: -1, never leaves). */
#include "xmen2.h"

void Ninja_Init(register Ninja *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_Ninja_vtable)->parent->method[3])(self, x, y, pl);	/* BIONPC Init */
	self->px = pl->x;
	self->py = pl->y;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_NinjaStats))->health;
	self->health = 1;
	if (g_NinjaSprDef2 == -1) {
		self->spr.def = CODEPTR(Art_NinjaSpr2);
		self->spr.flags = self->spr.flags & ~3 | 1;
	} else
		Obj_SetSpriteDef((Actor *)self, g_NinjaSprDef2);
	AiScript_Set(&self->ai, CODEPTR(Tbl_NinjaIntroScript));
	self->state = self->defaultState = (StateFn)Ninja_StateNextStep;
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
	Obj_StandOnSeg(self, Seg_FindNearestOfType(self->x >> 8, self->y >> 8, 0x3E9));
	Obj_Detach(self);
	STOP_MOVING(self);
	Obj_SetAnim(self, CODEPTR(Tbl_NinjaAnimIdle));
	if (pl->param[0])
		Ninja_CalcPatrolBounds(self, pl->param[0], &self->patrolLeft, &self->patrolRight);
	else
		Ninja_CalcPatrolBounds(self, 0xA0, &self->patrolLeft, &self->patrolRight);
	if (!pl->param[3])
		self->waitTime = 0x5A;
	else
		self->waitTime = pl->param[3];
	if (!pl->param[4])
		self->leaveCount = -1;
	else
		self->leaveCount = pl->param[4];
}
