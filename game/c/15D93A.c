/* Actor_InitSpawnRec - ACTOR InitSpawnRec (class slot +$0A): fill a spawn record from a level
 * placement entry.
 *
 * rec: the spawn record (Spawn_InitRecords).  pl: its placement entry.
 * The record waits to be spawned (flags 1), has no object yet ($FF), and keeps the placement's
 * position and the entry itself. */
#include "xmen2.h"

void Actor_InitSpawnRec(SpawnRec *rec, Placement *pl)
{
	rec->flags = 1;
	rec->object = 0xFF;
	rec->x = pl->x;
	rec->y = pl->y;
	rec->placement = pl;
}
