/* EvilMaker_Init - EVILMAKER Init (class slot +$12): the parent's (ACTOR) Init, then no motion,
 * its AI script Tbl_EvilMaker_ScrStepKillLen (the steps are the jmp thunks
 * EvilMaker_ScrStepKill...: kill, then capsule drops with idle and 60-frame waits between),
 * state and default state EvilMaker_StateWaitKills, flags6 |= $5083, and the sprite priority
 * bit copied from the live player (player 1 when neither is).
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry. */
#include "xmen2.h"

void EvilMaker_Init(EvilMaker *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_EvilMaker_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	AiScript_Set(&self->ai, CODEPTR(Tbl_EvilMaker_ScrStepKillLen));
	self->state = EvilMaker_StateWaitKills;
	self->defaultState = EvilMaker_StateWaitKills;
	self->flags6 |= 0x5083;
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
}
