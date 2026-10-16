/* EvilDrop_Init - EVILDROP Init (class slot +$12): the parent's (BIOBALLS) Init, then a falling
 * capsule: no motion, AI script Tbl_EvilDropAiScript (steps EvilDrop_ActOpen, EvilDrop_ActRelease),
 * state and default state EvilDrop_StateStart, flags6 |= $1083, opening path
 * Tbl_EvilDropDefaultBallPath, a copy of its spawn record (the clone to release), falling; ball 0
 * at its position and ball 1 gone (-1, -1) when the list has them; the sprite priority (bit 2) of
 * player 1 if alive, else of player 2 if alive, else player 1's; and the floor it lands on: the
 * higher end of the segment below the record's position (from 32 px down, layer 0), or $80 px
 * below it.
 *
 * self: the capsule.  x, y: position (8 fraction bits).  pl: its spawn record (never null:
 * EvilDrop_Spawn). Never runs in the corpus. */
#include "xmen2.h"

void EvilDrop_Init(register EvilDrop *self, long x, long y, Placement *pl)
{
	short px;
	short py;
	register BioBall *ball;
	register Seg *seg;

	(*CLASS(Tbl_EvilDrop_vtable)->parent->method[3])(self, x, y, pl);	/* BIOBALLS Init */
	STOP_MOVING(self);
	AiScript_Set(&self->ai, CODEPTR(Tbl_EvilDropAiScript));
	self->state = (StateFn)EvilDrop_StateStart;
	self->defaultState = (StateFn)EvilDrop_StateStart;
	self->flags6 |= 0x1083;
	self->openPath = CODEPTR(Tbl_EvilDropDefaultBallPath);
	self->clone = *pl;
	self->falling = 1;
	if (self->ballList && *self->ballList > 0) {
		ball = (BioBall *)(self->ballList + 1);
		ball->x = self->px;
		ball->y = self->py;
		if (*self->ballList > 1) {
			ball++;
			ball->x = -1;
			ball->y = -1;
		}
	}
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
	px = pl->x;
	py = pl->y;
	if ((seg = Seg_FindBelow(px, py + 0x20, 0)) != 0)
		self->floorY = MIN(seg->y1, seg->y2);
	else
		self->floorY = py + 0x80;
}
