/* SentinelDoor_Despawn - SENTINELDOOR Despawn (class slot +$36): the parent's (ACTOR) Despawn,
 * then free its doorway segment.
 *
 * self: the door.  rec: its spawn record (only passed on). */
#include "xmen2.h"

void SentinelDoor_Despawn(register SentinelDoor *self, SpawnRec *rec)
{
	(*CLASS(Tbl_SentinelDoor_vtable)->parent->method[12])(self, rec);	/* ACTOR Despawn */
	Seg_Free(self->doorSeg);
	self->doorSeg = 0;
}
