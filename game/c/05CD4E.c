/* Tusk_InitSpawnRec - TUSK's spawn record setup (class slot +$0A): the parent's (ACTOR), then
 * flag bit 3: spawn at once, without waiting to come into view.
 *
 * rec: the spawn record.  pl: its placement entry. */
#include "xmen2.h"

void Tusk_InitSpawnRec(SpawnRec *rec, Placement *pl)
{
	(*CLASS(Tbl_Tusk_vtable)->parent->method[1])(rec, pl);	/* ACTOR InitSpawnRec */
	rec->flags |= 8;
}
