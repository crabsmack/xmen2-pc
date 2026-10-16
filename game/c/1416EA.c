/* Tank_Init - TANK constructor (class slot +$12): the parent's (XNPC) Init, then a tank standing
 * at its placement, starting its AI script Tbl_Tank_ScriptWaitDrive at once.
 *
 * self: the new object.  x, y: position (8 fraction bits; the pixel position is the placement's).
 * pl: its placement entry (never null).
 * Stats Tbl_TankStats, but 8 health; sprite set g_TankSprDef; flags6 |= $80, $200; sprite
 * priority (bit 2) as the live player's (player 1's when neither is alive); not moving, the
 * animation Tbl_TankAnim. Placement params: 0 the track's half-length (0: 96; Tank_SetTrackLimits),
 * 1 speed << 8 (0: $100), 3 driveTime (0: 120). No rocket. */
#include "xmen2.h"

void Tank_Init(register Tank *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_Tank_vtable)->parent->method[3])(self, x, y, pl);	/* XNPC Init */
	self->px = pl->x;
	self->py = pl->y;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_TankStats))->health;
	self->health = 8;
	Obj_SetSpriteDef((Actor *)self, g_TankSprDef);
	AiScript_Set(&self->ai, CODEPTR(Tbl_Tank_ScriptWaitDrive));
	AI_FIRST_STEP(self, self->ai, 0);
	self->flags6 |= 0x80;
	self->flags6 |= 0x200;
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
	} else {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	}
	if (pl->param[0])
		Tank_SetTrackLimits(self, pl->param[0], &self->trackLeft, &self->trackRight);
	else
		Tank_SetTrackLimits(self, 0x60, &self->trackLeft, &self->trackRight);
	STOP_MOVING(self);
	Obj_SetAnim(self, CODEPTR(Tbl_TankAnim));
	if (!pl->param[1])
		self->speed = 0x100;
	else
		self->speed = pl->param[1] << 8;
	if (!pl->param[3])
		self->driveTime = 0x78;
	else
		self->driveTime = pl->param[3];
	self->rocket = 0;
}
