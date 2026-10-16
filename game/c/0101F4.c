/* BigAcolyte_Init - BIGACOLYTE constructor (class slot +$12): the parent's (BIONPC) Init, then
 * the boss set up to wait behind its wall (the intro AI script).
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * 24 health, attack 1, sprite Art_BigAcolyteSpr with palette 1, the intro AI script
 * (Tbl_BigAcolyteIntroScript: BigAcolyte_IntroScript, 6 steps WaitPlayers, Appear,
 * MoveToWall, Grow, Wait60, StartFight) from its first step, flags6 |= $1082, sprite priority copied from
 * the live player (player 1 when neither is). No motion, speed limit +/-$280, both axes arrived,
 * not bobbing, not a picture, height level 0, countdown 1000. */
#include "xmen2.h"

void BigAcolyte_Init(register BigAcolyte *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BigAcolyte_vtable)->parent->method[3])(self, x, y, pl);	/* BIONPC Init */
	self->health = 24;
	self->attack = 1;
	self->spr.def = CODEPTR(Art_BigAcolyteSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	AiScript_Set(&self->ai, CODEPTR(Tbl_BigAcolyteIntroScript));
	AI_FIRST_STEP(self, self->ai, 0);
	self->flags6 |= 0x1082;
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
	STOP_MOVING(self);
	self->bobAngle = 0;
	self->maxVy = 0x280;		/* its speed limits, both axes (BigAcolyte_Move) */
	self->minVy = -0x280;
	self->unk_9E = 0;
	self->arrivedY = 1;
	self->arrivedX = 1;
	self->bobbing = 0;
	self->picture = 0;
	self->countdown = 1000;
	self->level = 0;
}
