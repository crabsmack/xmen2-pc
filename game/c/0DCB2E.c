/* Blast_Spawn - create a BLAST (its New, class slot +$0E) at a spawn placement's position, with the
 * placement as its parameters (BLAST Init reads them).
 *
 * rec: the placement (pixels; ControlBox_FireBlast and the control panels build one on their
 * stack). The new object is kept in a variable and never used. */
#include "xmen2.h"

void Blast_Spawn(register Placement *rec)
{
	register long x = rec->x << 8;
	register long y = rec->y << 8;
	register Blast *b;

	b = (Blast *)(*CLASS(Tbl_Blast_vtable)->method[2])(0, CLASS(Tbl_Blast_vtable), x, y, rec);	/* New */
}
