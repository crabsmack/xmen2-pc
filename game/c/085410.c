/* TeleportReceiver_InitSpawnRec - TELEPORTRECEIVER's spawn record setup (class slot +$0A): the
 * parent's (ACTOR), then flags 0: the record never spawns an object (a receiver is only its
 * placement, which Teleporter_Init finds through g_SpawnRecs).
 *
 * rec: the spawn record.  pl: its placement entry. */
#include "xmen2.h"

void TeleportReceiver_InitSpawnRec(register SpawnRec *rec, Placement *pl)
{
	register SpawnRec *r = rec;

	(*CLASS(Tbl_TeleportReceiver_vtable)->parent->method[1])(rec, pl);	/* ACTOR InitSpawnRec */
	r->flags = 0;
}
