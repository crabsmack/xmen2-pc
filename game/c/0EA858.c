/* Hologram_InitSpawnRec - HOLOGRAM InitSpawnRec (class slot +$0A, static): fill the spawn record
 * as the parent (ACTOR) does, then set its bit 3 so the level's spawn pass creates the hologram
 * at once, wherever the camera is.
 *
 * rec: the spawn record.  pl: its placement entry. */
#include "xmen2.h"

void Hologram_InitSpawnRec(register SpawnRec *rec, Placement *pl)
{
	(*CLASS(Tbl_Hologram_vtable)->parent->method[1])(rec, pl);	/* ACTOR InitSpawnRec */
	rec->flags |= 8;
}
