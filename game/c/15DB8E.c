/* Actor_SpawnFromRec - ACTOR SpawnFromRec (class slot +$2E): create the object of a spawn record.
 *
 * self: the spawn record the slot was called on (its class at +0).  rec: the same record.
 * The class's New (slot +$0E) makes the object at the record's position (pixels << 8) with its
 * placement entry. The object keeps the record's index (unk_14: the spawn scan's g_SpawnScanLeft,
 * whose low byte the original read), the record its object index, and the record becomes live
 * (flags bit 0 off, bit 1 on). Returns the object, or 0 when none was free. */
#include "xmen2.h"

Actor *Actor_SpawnFromRec(SpawnRec *self, register SpawnRec *rec)
{
	long x, y;
	Actor *o;

	x = rec->x << 8;
	y = rec->y << 8;
	o = (Actor *)(*self->cls->method[2])(0, rec->cls, x, y, rec->placement);
	if (o) {
		o->unk_14 = (u8)g_SpawnScanLeft;
		rec->object = o->index;
		rec->flags &= ~1;
		rec->flags |= 2;
	}
	return o;
}
