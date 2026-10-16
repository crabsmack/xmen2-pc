/* SpikeBall_Init - SPIKEBALL Init (class slot +$12): the parent's (ACTOR) Init, then a spike
 * ball that cycles its spikes in and out (SpikeBall_StateCycle).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry.
 * flags6 |= $4082, damage 1, the class's sprite set (g_SpikeBallSprDef). Placement params: +6
 * cycle length as a power of two (0: 6, 64 frames; mask = 2^n - 1), +8 frames out per cycle
 * (0: a quarter of the mask), the low byte of +$A the cycle's phase. State and default state
 * SpikeBall_StateCycle.
 * (The if (1) block ends in a label where MPW C pops the pending call arguments, as the ROM.) */
#include "xmen2.h"

void SpikeBall_Init(register SpikeBall *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_SpikeBall_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->flags6 |= 0x4082;
	self->attack = 1;
	if (1) {
		Obj_SetSpriteDef((Actor *)self, g_SpikeBallSprDef);
	}
	self->mask = (1L << (pl->param[0] ? pl->param[0] : 6)) - 1;
	self->onTime = pl->param[1] ? pl->param[1] : self->mask >> 2;
	self->phase = pl->param[2];
	self->state = self->defaultState = SpikeBall_StateCycle;
}
