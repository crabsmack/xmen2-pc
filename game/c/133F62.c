/* FireWall_Init - FIREWALL Init (class slot +$12): the parent's (ACTOR) Init, not moving, state
 * (and default state) FireWall_StateWaitOnScreen, flags6 |= $83, priority bit (2) from the live
 * player (player 1 when neither is); from the placement: the chain's repeat delay (param 0, 0:
 * 120), its length (param 1: 0-2, other 0) and way (param 2: 0 rises, else drops); no flame.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (read without a
 * test for 0). */
#include "xmen2.h"

void FireWall_Init(FireWall *self, long x, long y, Placement *pl)
{
	register short v;		/* the placement param being read */

	(*CLASS(Tbl_FireWall_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	self->state = (StateFn)FireWall_StateWaitOnScreen;
	self->defaultState = (StateFn)FireWall_StateWaitOnScreen;
	self->flags6 |= 0x83;
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
#ifndef XMEN2_MPW
	if (!pl)	/* no placement: the 68000 reads the params from the vector table */
		pl = (Placement *)Vectors;
#endif
	self->delay = (v = pl->param[0]) ? v : 0x78;
	v = pl->param[1];
	self->size = (v >= 0 && v < 3) ? v : 0;
	self->rise = !pl->param[2] ? 1 : 0;
	self->flame = 0;
}
