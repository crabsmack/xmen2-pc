/* ExodusBlast_Spawn - create an EXODUSBLAST (its New, class slot +$0E) at (x, y) and give it
 * its kind (1-7: the direction, ExodusBlast_StateLaunch).
 *
 * x, y: position (8 fraction bits).  kind: what to fly.
 * Returns the blast, 0 when none could be made. */
#include "xmen2.h"

ExodusBlast *ExodusBlast_Spawn(long x, long y, char kind)
{
	register ExodusBlast *blast;

	blast = (ExodusBlast *)(*CLASS(Tbl_ExodusBlast_vtable)->method[2])(0,	/* New */
		CLASS(Tbl_ExodusBlast_vtable), x, y, 0);
	if (blast)
		blast->kind = kind;
	return blast;
}
