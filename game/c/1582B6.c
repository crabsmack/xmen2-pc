/* FluxBomb_Init - FLUXBOMB constructor (class slot +$12): the parent's (RTHETAOBJECT) Init, then a
 * bomb Magneto holds (FluxBomb_StartHeld; on the players' side, flags6 $10) or one the clone
 * throws (FluxBomb_StartThrown).
 *
 * self: the bomb.  x, y: position (8 fraction bits).
 * pl: placement params: param 0's low byte, fired by the player.
 * Top speed $400; particle 0 (0, 200, 200); active; flags6 |= $108A; no particles, not strong
 * (Magneto_SpawnFluxBomb sets it after). The shared sprite g_FluxBombSpr gets the (first live)
 * player's priority; the object's own sprite none (it draws g_FluxBombSpr). */
#include "xmen2.h"

void FluxBomb_Init(register FluxBomb *self, long x, long y, register Placement *pl)
{
	register char byPlayer = (char)pl->param[0];

	(*CLASS(Tbl_FluxBomb_vtable)->parent->method[3])(self, x, y, pl);	/* RTHETAOBJECT Init */
	self->maxSpeed = 0x400;
	self->part[0].flags = 0;
	self->part[0].x = 200;
	self->part[0].y = 200;
	if (byPlayer) {
		self->state = FluxBomb_StartHeld;
		self->defaultState = FluxBomb_StartHeld;
	} else {
		self->state = FluxBomb_StartThrown;
		self->defaultState = FluxBomb_StartThrown;
	}
	self->flags &= ~0x4000;
	self->flags6 |= 0x108A;
	if (byPlayer)
		self->flags6 |= 0x10;
	self->phase = 0;
	self->strong = 0;
	if (IS_ALIVE(&g_Player1)) {
		if (g_Player1.spr.flags & 4)
			g_FluxBombSpr.flags |= 4;
		else
			g_FluxBombSpr.flags &= ~4;
	} else if (IS_ALIVE(&g_Player2)) {
		if (g_Player2.spr.flags & 4)
			g_FluxBombSpr.flags |= 4;
		else
			g_FluxBombSpr.flags &= ~4;
	} else {
		if (g_Player1.spr.flags & 4)
			g_FluxBombSpr.flags |= 4;
		else
			g_FluxBombSpr.flags &= ~4;
	}
	self->spr.def = 0;
	(*self->cls->method[20])(self, 0xFF);			/* +$56 SetFrame: none */
}
