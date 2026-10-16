/* EvilDrop_Spawn - make an EVILDROP capsule at a spawn record's position through its class's New
 * (class slot +$0E); the record (its param 0 the clone type) becomes the capsule's placement.
 *
 * pl: the record (x, y in pixels).  unused: a ball path the EVILMAKER callers pass, not read.
 * Returns the capsule (0: no room). Never runs in the corpus. */
#include "xmen2.h"

EvilDrop *EvilDrop_Spawn(register Placement *pl, void *unused)
{
	register EvilDrop *drop;

	drop = (EvilDrop *)(*CLASS(Tbl_EvilDrop_vtable)->method[2])(0, CLASS(Tbl_EvilDrop_vtable),	/* New */
		pl->x << 8, pl->y << 8, pl);
	return drop;
}
