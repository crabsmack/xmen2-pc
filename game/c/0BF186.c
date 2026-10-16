/* SporeMine_SpawnFromRec - SPOREMINE SpawnFromRec (class slot +$2E): a spore in the pool for a
 * spawn record instead of an object.
 *
 * self: the spawn record the slot was called on (unused).  rec: the same record.
 * The spore keeps the record; the record gets no object ($FF) and becomes live (flags bit 0
 * off, bit 1 on). Returns 0 (no object), also when the pool is full (the record then stays
 * waiting). */
#include "xmen2.h"

Actor *SporeMine_SpawnFromRec(SpawnRec *self, register SpawnRec *rec)
{
	register Spore *s;

	s = SporeMine_AddSpore(rec->placement);
	if (s) {
		s->rec = rec;
		rec->object = 0xFF;
		rec->flags &= ~1;
		rec->flags |= 2;
	}
	return 0;
}
