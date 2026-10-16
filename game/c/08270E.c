/* DropBomb_Spawn - make a DROPBOMB at (x, y) through its class's New (class slot +$0E).
 *
 * x, y: position (8 fraction bits).  pl: its placement entry, or null (BROODBOSS passes 0: then
 * DropBomb_Init uses a $78-frame drop delay).
 * Returns the bomb (0: no room). */
#include "xmen2.h"

DropBomb *DropBomb_Spawn(long x, long y, Placement *pl)
{
	register DropBomb *bomb;

	bomb = (DropBomb *)(*CLASS(Tbl_DropBomb_vtable)->method[2])(0, CLASS(Tbl_DropBomb_vtable), x, y, pl);	/* New */
	return bomb;
}
