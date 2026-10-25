/* ToxicCan_Init - TOXICCAN constructor (class slot +$12): the parent's (ACTOR) Init, then a still
 * barrel with 4 health, in front of or behind sprites as the (first live) player is.
 *
 * self: the new can.  x, y: position (8 fraction bits).  pl: its placement entry (param 2: the
 * draw layer).
 * Sprite definition Art_ToxicCanSpr, palette 1; flags6 |= $80, 1, $200 (no collisions, no attack
 * node, owns floor segments: it can be stood on); not in the body hit list yet (Activate). */
#include "xmen2.h"

void ToxicCan_Init(register ToxicCan *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_ToxicCan_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->spr.def = CODEPTR(Art_ToxicCanSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->health = 4;
	self->flags6 |= 0x80;
	self->flags6 |= 1;
	self->flags6 |= 0x200;
	STOP_MOVING(self);
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
	self->layer = (u8)pl->param[2];
	self->playerBodyNode = 0;
	Obj_UpdateBoxes(self);
}
