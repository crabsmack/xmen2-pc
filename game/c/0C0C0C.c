/* SwampFly_Init - SWAMPFLY Init (class slot +$12): the parent's (BIONPC) Init, then a fly at its
 * placement, flapping, that starts its flight from the next update (SwampFly_StateStart).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry.
 * flags6 |= $4080, stats Tbl_MaintRobotStats (health from them), sprite Art_SwampFlySpr (a
 * sprite definition despite its name), priority bit, palette 1; home and position the
 * placement's. Placement: the low byte of param 0 (+7) its path kind, of param 1 (+9) its step
 * time, params 2 and 3 + 8 its path scale shifts (all read only when pl is not 0, though pl was
 * already read). No target, flap shift 2; state and default state SwampFly_StateStart.
 * (The do/while (0) puts the loop label where MPW C pops the parent Init's arguments, as the
 * ROM.) */
#include "xmen2.h"

void SwampFly_Init(register SwampFly *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_SwampFly_vtable)->parent->method[3])(self, x, y, pl);	/* BIONPC Init */
	self->flags6 |= 0x4080;
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_MaintRobotStats))->health;
	do {
		self->spr.def = CODEPTR(Art_SwampFlySpr);
		self->spr.flags |= 4;
		self->spr.flags = self->spr.flags & ~3 | 1;
	} while (0);
	self->px = pl->x;
	self->py = pl->y;
	self->x = pl->x << 8;
	self->y = pl->y << 8;
	if (pl) {
		self->pathKind = pl->param[0];
		self->stepTime = pl->param[1];
		self->shiftX = pl->param[2] + 8;
		self->shiftY = pl->param[3] + 8;
	}
	self->unk_B3 = 0;
	self->target = 0;
	self->flapShift = 2;
	self->state = self->defaultState = SwampFly_StateStart;
}
