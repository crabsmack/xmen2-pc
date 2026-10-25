/* HoloBolt_Init - HOLOBOLT constructor (class slot +$12): the parent's (ACTOR) Init, then flags6
 * |= $5082 (frozen until HoloBolt_StateActivate), sprite set g_HoloBoltSprDef, the sprite
 * priority bit (spr.flags bit 2) of player 1 if alive, else of player 2 if alive, else of
 * player 1; state (and default state) HoloBolt_StateAppear.
 *
 * self: the new bolt.  x, y: position (8 fraction bits).  pl: placement entry (only passed on). */
#include "xmen2.h"

void HoloBolt_Init(register HoloBolt *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_HoloBolt_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->flags6 |= 0x5082;
	Obj_SetSpriteDef((Actor *)self, g_HoloBoltSprDef);
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
	self->state = self->defaultState = HoloBolt_StateAppear;
}
