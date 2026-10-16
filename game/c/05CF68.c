/* Tusk_Init - TUSK Init (class slot +$12): the parent's (ACTOR) Init, then flags6 |= $1182,
 * attack 1, health 30, no motion, first state Tusk_StateIntro, sprite Art_TuskSpr with palette 3
 * (set twice), its two floor segments from 32 px below the placement point (its ledge: the
 * nearest above, the arena floor: the nearest below, layers up to 1000), not hittable.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry.
 * The do/while (0) blocks are the original's (macros, most likely): their labels are where
 * MPW C pops the calls' arguments. hittable and unread_9B are cleared by one clr.w. */
#include "xmen2.h"

void Tusk_Init(Tusk *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_Tusk_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->flags6 |= 0x1182;
	self->attack = 1;
	self->health = 30;
	STOP_MOVING(self);
	self->state = Tusk_StateIntro;
	do {
		self->spr.def = CODEPTR(Art_TuskSpr);
		self->spr.flags = self->spr.flags & ~3 | 3;
	} while (0);
	do {
		self->spr.def = CODEPTR(Art_TuskSpr);
		self->spr.flags = self->spr.flags & ~3 | 3;
	} while (0);
	do {
		short px = pl->x;
		short py = pl->y + 32;

		self->ledge = Seg_FindAbove(px, py, 1000);
		self->arenaFloor = Seg_FindBelow(px, py, 1000);
	} while (0);
	self->hittable = 0;
	self->unread_9B = 0;
}
