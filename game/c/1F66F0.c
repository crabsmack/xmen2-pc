/* Actor_DeactivateFree - Deactivate of BRAINROCKET, NINJA and TANKLEFT (class slot +$46): take
 * the object out of play for good.
 *
 * obj: the object (any Actor class).
 * ACTOR's Deactivate, then the object becomes unused (flags 0) and its timer is freed; its spawn
 * record (spawnRec, when not $FF) is no longer live (flags bit 1 off) and has no object ($FF), so
 * the spawn pass never despawns or respawns it while it waits. */
#include "xmen2.h"

void Actor_DeactivateFree(void *obj)
{
	register Actor *self = (Actor *)obj;

	(*CLASS(Tbl_Actor_vtable)->method[16])(self);		/* Deactivate */
	self->flags = 0;
	Timer_Free(self->timerObj);
	self->timerObj = 0;
	if (self->spawnRec != 0xFF) {
		g_SpawnRecs[self->spawnRec].flags &= ~2;
		g_SpawnRecs[self->spawnRec].object = 0xFF;
	}
}
