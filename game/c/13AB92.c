/* Mine_SpawnFromRec - MINE SpawnFromRec (class slot +$2E): a mine in the pool for a spawn record
 * instead of an object.
 *
 * self: what the slot was called on (unused).  rec: the spawn record.
 * The mine keeps the record; the record gets no object ($FF) and becomes live (flags bit 0
 * off, bit 1 on). Returns 0 (no object), also when the pool is full (the record then stays
 * waiting). */
#include "xmen2.h"

Actor *Mine_SpawnFromRec(SpawnRec *self, register SpawnRec *rec)
{
	register MineRec *m;

	m = Mine_PoolAdd(rec->placement);
	if (m) {
		m->rec = rec;
		rec->object = 0xFF;
		rec->flags &= ~1;
		rec->flags |= 2;
	}
	return 0;
}
