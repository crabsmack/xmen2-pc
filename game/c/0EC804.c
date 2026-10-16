/* CoreElevator_InitSpawnRec - COREELEVATOR's spawn record setup (class slot +$0A): the parent's
 * (ACTOR), then flag bit 3: spawn at once, wherever the camera is.
 *
 * rec: the spawn record.  pl: its placement entry. */
#include "xmen2.h"

void CoreElevator_InitSpawnRec(register SpawnRec *rec, Placement *pl)
{
	(*CLASS(Tbl_CoreElevator_vtable)->parent->method[1])(rec, pl);	/* ACTOR InitSpawnRec */
	rec->flags |= 8;
}
