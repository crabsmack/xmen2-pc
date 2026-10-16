/* SyncActor_MemberSpawnRecOutOfView - SYNCHRONIZEDACTOR slot +$76: the parent's (ACTOR)
 * SpawnRecOutOfView test (slot +$32), for the SYNCHRONIZER (Synchronizer_WaitMembers).
 *
 * self: the object.  rec: its spawn record.
 * Returns the parent's answer. */
#include "xmen2.h"

char SyncActor_MemberSpawnRecOutOfView(Synchronizedactor *self, SpawnRec *rec)
{
	return (char)(*CLASS(Tbl_SyncActor_vtable)->parent->method[11])(self, rec);	/* ACTOR SpawnRecOutOfView */
}
