/* Barrier_Init - BARRIER constructor (class slot +$12): the parent's (ACTOR) Init, then an open
 * barrier waiting to close behind the players (Barrier_StateStart).
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry.
 * Not moving; position = the placement's (pixels); state (and default state)
 * Barrier_StateStart; flags6 |= $83; sprite set g_BarrierSprDef, palette 0; its priority bit
 * (sprite flag 4) from the live player (player 1 when neither is). From the placement: trigger
 * (param 0), height in 32-px blocks (param 1, 0: 4); the closed block spans the placement's y
 * (bottom) up height * 32 px (top). No side seen yet, open; no frame shown ($FF).
 * (The SetFrame call is in a do/while (0), as a macro: MPW keeps its argument pop, and the
 * loop makes it pop the earlier calls' arguments before the first branch, as the ROM does.) */
#include "xmen2.h"

void Barrier_Init(register Barrier *self, long x, long y, register Placement *pl)
{
	register short v;

	(*CLASS(Tbl_Barrier_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	self->px = pl->x;
	self->py = pl->y;
	self->state = Barrier_StateStart;
	self->defaultState = Barrier_StateStart;
	self->flags6 |= 0x83;
	Obj_SetSpriteDef((Actor *)self, g_BarrierSprDef);
	self->spr.flags = self->spr.flags & ~3 | 0;
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
	self->trigger = pl->param[0];
	self->bottom = pl->y;
	self->top = self->bottom - ((self->height = (v = pl->param[1]) ? v : 4) << 5);
	self->side = 0;
	self->closed = 0;
	do {
		(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: none */
	} while (0);
}
