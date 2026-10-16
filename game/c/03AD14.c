/* RollingDiscs_Init - ROLLINGDISCS Init (class slot +$12): the parent's (ACTOR) Init, then it is
 * the scene's rolling-discs object (g_RollingDiscsObj), flags6 |= $1082, no vertical speed or
 * gravity, its pixel position from the placement, attack 10, 2 health, layer 0, x speed from the
 * placement's first parameter ($200 without one) with its size as the maximum, +$8B 0, the
 * bands (g_DiscBands), state and default state RollingDiscs_InitStub.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry.
 * Never runs in the corpus. The object pointer at $FFC588 has no name in the disassembly
 * (-$3C78, the word after g_DiscBandCount): the macro reaches it from there.
 * MPW: the label at the end is where the ROM pops the call's arguments. */
#include "xmen2.h"

#define g_RollingDiscsObj	(*(PTR(RollingDiscs) *)(&g_DiscBandCount + 1))	/* $FFC588 */

void RollingDiscs_Init(register RollingDiscs *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_RollingDiscs_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	g_RollingDiscsObj = self;
	self->flags6 |= 0x1082;
	self->vy = 0;
	self->ay = 0;
	self->px = pl->x;
	self->py = pl->y;
	self->attack = 10;
	self->health = 2;
	self->layer = 0;
	self->vx = pl->param[0] ? pl->param[0] : 0x200;
	self->maxVx = ABS(self->vx);
	self->unk_8A[1] = 0;
	self->bands = g_DiscBands;
	self->state = self->defaultState = (StateFn)RollingDiscs_InitStub;
end:	;
}
