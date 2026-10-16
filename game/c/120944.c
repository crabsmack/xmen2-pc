/* BioSentinel_Deactivate - BIOSENTINEL Deactivate (class slot +$46): the parent's
 * (BASICBIOSENTINEL); a ceiling sentinel (variant 1) is then gone for good: flags cleared, its
 * timer freed, and its spawn record (spawnRec, unless $FF) no longer live and without an object,
 * so it is never spawned again.
 *
 * self: the sentinel. */
#include "xmen2.h"

void BioSentinel_Deactivate(register BioSentinel *self)
{
	(*CLASS(Tbl_BioSentinel_vtable)->parent->method[16])(self);	/* BASICBIOSENTINEL Deactivate */
	if (self->variant == 1) {
		self->flags = 0;
		Timer_Free(self->timerObj);
		self->timerObj = 0;
		if (self->spawnRec != 0xFF) {
			g_SpawnRecs[self->spawnRec].flags &= ~2;
			g_SpawnRecs[self->spawnRec].object = 0xFF;
		}
	}
}
