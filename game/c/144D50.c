/* ToxicCanDamaged_Init - TOXICCANDAMAGED constructor (class slot +$12): a TOXICCAN (its Init) that
 * was damaged before it left the screen, back with only 2 health (leaking).
 *
 * self: the new can.  x, y: position (8 fraction bits).  pl: its placement entry. */
#include "xmen2.h"

void ToxicCanDamaged_Init(register ToxicCanDamaged *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_ToxicCanDamaged_vtable)->parent->method[3])(self, x, y, pl);	/* TOXICCAN Init */
	self->health = 2;
}
