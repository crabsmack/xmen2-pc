/* SentinelElevator_Init - SENTINELELEVATOR constructor (class slot +$12): the parent's (ACTOR)
 * Init, then a lift waiting for its switch.
 *
 * self: the new object.  x, y: position (8 fraction bits).
 * pl:   its placement entry, or 0 (then its switch fields and ride range are not set).
 * No vertical speed or gravity, state (and default state) SentinelElevator_StateStart, flags6
 * |= $52A3, layer 4, sprite definition Art_SentinelElevatorSpr (set directly), palette 0, 20
 * frames' wait at either end. Placement: param 0 the switch number (out of 0-14: 0; its flag
 * word g_SentinelsD1Flags[switch]), the ride from y - 32 * param 1 up to y + 32 * param 2 (0:
 * the placement's y), param 4 = 99: automatic (SentinelElevator_StateAuto, no wait at the top,
 * layer 0, flags6 bit $1000 cleared). Then boxes updated (Obj_UpdateBoxes). */
#include "xmen2.h"

void SentinelElevator_Init(register SentinelElevator *self, long x, long y,
	register Placement *pl)
{
	register short v;

	(*CLASS(Tbl_SentinelElevator_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->state = SentinelElevator_StateStart;
	self->defaultState = SentinelElevator_StateStart;
	self->flags6 |= 0x52A3;
	self->layer = 4;
	self->spr.def = CODEPTR(Art_SentinelElevatorSpr);
	self->spr.flags = self->spr.flags & ~3 | 0;
	self->topWait = 20;
	self->bottomWait = 20;
	self->unread_9A = 0;
	if (pl) {
		v = pl->param[0];
		if (v < 0 || v >= 15)
			v = 0;
		self->switchNum = v;
		self->switchFlags = &g_SentinelsD1Flags[v];
		self->topY = self->bottomY = pl->y;
		if (v = pl->param[1])
			self->topY = pl->y - (v << 5);
		if (v = pl->param[2])
			self->bottomY = pl->y + (pl->param[2] << 5);
		if (pl->param[4] == 99) {
			self->state = SentinelElevator_StateAuto;
			self->defaultState = SentinelElevator_StateAuto;
			self->topWait = 0;
			self->layer = 0;
			self->flags6 &= ~0x1000;
		}
	}
	self->unread_92 = 0;
	Obj_UpdateBoxes(self);
end:	;	/* (a label: MPW C pops the calls' arguments before it and before the if, as the ROM) */
}
