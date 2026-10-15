/* Exodus_InitSpawnRec - EXODUS's spawn record setup (class slot +$0A): the parent's (ACTOR),
 * then flag bit 3: spawn at once, without waiting to come into view.
 *
 * rec: the spawn record.  pl: its placement entry. */
#include "xmen2.h"

void Exodus_InitSpawnRec(register SpawnRec *rec, Placement *pl)
{
	(*CLASS(Tbl_Exodus_vtable)->parent->method[1])(rec, pl);	/* ACTOR InitSpawnRec */
	rec->flags |= 8;
}
