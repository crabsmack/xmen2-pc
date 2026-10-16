/* MaintRobot_Init - MAINTROBOT constructor (class slot +$12): the parent's (XNPC) Init, then a
 * robot about to drive (MaintRobot_StateResume, also its default state).
 *
 * self: the new object.  x, y: position (8 fraction bits; replaced by the placement's).
 * pl: its placement entry (never null).
 * At the placement's position, no gravity; sprite set g_MaintRobotSprDef (or, when that is -1,
 * Art_MaintRobotSpr itself, palette 1); flags6 |= $80; stats Tbl_MaintRobotStats (health from
 * them); layer $10. Placement params: 0 speed << 8 (0: $200; also its first velocity and patrol
 * velocity), 1 fireDelay (0: 60), 2 volley (0: 2), 3 shotDelay (0: 20), 4 patrol (0: chase).
 * Sprite priority (bit 2) set when the live player's (player 2's when player 1 is not alive)
 * is; standing on the segment where it is (Obj_StandOnSegHere). */
#include "xmen2.h"

void MaintRobot_Init(register MaintRobot *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_MaintRobot_vtable)->parent->method[3])(self, x, y, pl);	/* XNPC Init */
	self->x = (long)pl->x << 8;
	self->y = (long)pl->y << 8;
	self->ay = 0;
	self->state = self->defaultState = MaintRobot_StateResume;
	if (g_MaintRobotSprDef == -1) {
		self->spr.def = CODEPTR(Art_MaintRobotSpr);
		self->spr.flags = self->spr.flags & ~3 | 1;
	} else
		Obj_SetSpriteDef((Actor *)self, g_MaintRobotSprDef);
	self->flags6 |= 0x80;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_MaintRobotStats))->health;
	self->layer = 0x10;
	self->unread_98 = 0;
	self->speed = pl->param[0] ? (long)pl->param[0] << 8 : 0x200;
	self->fireDelay = pl->param[1] ? pl->param[1] : 0x3C;
	self->volley = pl->param[2] ? pl->param[2] : 2;
	self->shotDelay = pl->param[3] ? pl->param[3] : 0x14;
	self->patrol = pl->param[4] ? pl->param[4] : 0;
	self->shotsLeft = 0;
	self->unread_A6 = 0;
	self->patrolVx = self->speed;
	self->vx = self->speed;
	self->unread_94 = 0;
	if ((IS_ALIVE(&g_Player1) ? &g_Player1 : &g_Player2)->spr.flags & 4)
		self->spr.flags |= 4;
	Obj_StandOnSegHere(self);
}
