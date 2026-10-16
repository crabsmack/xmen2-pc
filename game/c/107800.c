/* Switch_InitSpawnRec - SWITCH's spawn record setup (class slot +$0A): the parent's (ACTOR), then
 * the record's flags 0: the level never spawns a SWITCH (its placements only mark switch tiles for
 * Switch_FindAndToggle).
 *
 * rec: the spawn record.  pl: its placement entry. */
#include "xmen2.h"

void Switch_InitSpawnRec(register SpawnRec *rec, Placement *pl)
{
	register SpawnRec *r = rec;

	(*CLASS(Tbl_Switch_vtable)->parent->method[1])(rec, pl);	/* ACTOR InitSpawnRec */
	r->flags = 0;
}
