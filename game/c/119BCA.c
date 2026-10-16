/* Synchronizer_GatherMembers - SYNCHRONIZER state after Init: look at up to 10 spawn records a
 * frame for the members (class memberClass, placement param 0 = its board entry); spawn each one
 * not live (its class's slot +$2E, keeping the record's index in the object's spawnRec), activate it
 * if it isn't (slot +$3E) and add it (up to 10). Past the last record: run the members
 * (Synchronizer_RunMembers).
 *
 * self: the synchronizer.
 * The record past the last is read before the end test. A member whose record has no object
 * ($FF) would be taken as object 0. */
#include "xmen2.h"

void Synchronizer_GatherMembers(register Synchronizer *self)
{
	Actor *spawned;
	Actor *obj;
	Placement *pl;
	register short n;
	register short i;
	register SpawnRec *rec;

	for (n = 10; n; --n) {
		i = self->scan++;
		rec = &g_SpawnRecs[i];
		pl = rec->placement;
		if (i >= g_SpawnRecCount) {
			self->state = Synchronizer_RunMembers;
			return;
		}
		if (((ClassList *)g_ClassList)->cls[pl->classIndex] == self->memberClass
		    && pl->param[0] == self->board) {
			if (!(rec->flags & 2)) {
				spawned = (Actor *)(*rec->cls->method[10])(rec, rec);	/* +$2E SpawnFromRec */
				spawned->spawnRec = (u8)i;
			}
			obj = rec->object == 0xFF ? 0 : g_ObjTable[rec->object];
			if (!IS_ALIVE(obj))
				(*obj->cls->method[14])(obj);		/* +$3E Activate */
			if (self->memberCount < 10)
				self->member[self->memberCount++] = obj;
		}
	}
}
