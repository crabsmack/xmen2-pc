/* BioBalls_Init - BIOBALLS Init (class slot +$12): the parent's (ACTOR) Init, then a group of
 * balls drawn from the shared ball list, collapsed on the object (BioBalls_ActCollapse) and idle.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * Not moving, AI script Tbl_BioBalls_ScriptIdle (one step, the thunk BioBalls_ScriptIdle:
 * BioBalls_StateIdle), state and default state BioBalls_ActCollapse, flags6 |= $1083, sprite
 * Art_BioBallsSpr with palette 1, priority bit (2)
 * from the live player (player 1 when neither is), ball list g_BioBallsList, no flags, not done,
 * no path, no path offset, no motion, the box a point at the position. */
#include "xmen2.h"

void BioBalls_Init(BioBalls *self, long x, long y, Placement *pl)
{
	short px = (short)(x >> 8);
	short py = (short)(y >> 8);

	(*CLASS(Tbl_BioBalls_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	AiScript_Set(&self->ai, CODEPTR(Tbl_BioBalls_ScriptIdle));
	self->state = (StateFn)BioBalls_ActCollapse;
	self->defaultState = (StateFn)BioBalls_ActCollapse;
	self->flags6 |= 0x1083;
	self->spr.def = CODEPTR(Art_BioBallsSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
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
	self->ballList = g_BioBallsList;
	self->ballFlags = 0;
	self->done = 0;
	self->path = 0;
	self->pathDx = 0;
	self->pathDy = 0;
	self->motion = 0;
	self->boxLeft = px;
	self->boxRight = px;
	self->boxTop = py;
	self->boxBottom = py;
}
