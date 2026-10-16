/* Synchronizer_DespawnMembers - SYNCHRONIZER state, all members out of view: despawn each one
 * (its slot +$36, with its spawn record), put its spawn record back at its placement position,
 * then destroy the synchronizer (slot +$1E).
 *
 * self: the synchronizer. */
#include "xmen2.h"

void Synchronizer_DespawnMembers(Synchronizer *self)
{
	register short i;
	register Actor *m;
	register short r;
	register SpawnRec *rec;
	Placement *pl;

	for (i = 0; i < self->memberCount; i++) {
		m = self->member[i];
		r = m->unk_14;			/* its spawn record */
		rec = &g_SpawnRecs[r];
		pl = rec->placement;
		(*m->cls->method[12])(m, rec);		/* +$36 Despawn */
		rec->x = pl->x;
		rec->y = pl->y;
	}
	(*self->cls->method[6])(self);			/* +$1E Destroy */
}
