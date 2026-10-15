/* Barbarian_Init - BARBARIAN constructor (class slot +$12): the parent's (BIONPC) Init, then
 * stats, sprite and walking speed, and the floor segments it can reach.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry (or 0).
 * Stats Tbl_BarbarianStats (health from it), flags6 |= $4080, sprite Art_BarbarianSpr with
 * palette 1, no target, walkSpeed 900 or the placement's param 0 when nonzero; its platform and
 * neighbour segments (Barbarian_FindPlatforms); not walking, segment not gone, no taunts; first
 * state Barbarian_StateStart (timer and default state left as the parent set them).
 * The empty do/while (0) is a macro that expands to nothing, as the original's must have: its
 * loop label is where MPW C pops the parent Init's arguments, and a function with a loop keeps
 * its last pop (Barbarian_FindPlatforms's) before returning, both as in the ROM. */
#include "xmen2.h"

void Barbarian_Init(Barbarian *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_Barbarian_vtable)->parent->method[3])(self, x, y, pl);	/* BIONPC Init */
	self->health = (self->stats = (NpcStats *)CODEPTR(Tbl_BarbarianStats))->health;
	do {
	} while (0);
	self->flags6 |= 0x4080;
	self->spr.def = CODEPTR(Art_BarbarianSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->target = 0;
	self->walkSpeed = 900;
	if (pl && pl->param[0])
		self->walkSpeed = pl->param[0];
	Barbarian_FindPlatforms(self, pl);
	self->walking = 0;
	self->segGone = 0;
	self->state = Barbarian_StateStart;
	self->taunts = 0;
}
