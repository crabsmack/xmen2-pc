/* BeamScooter_Spawn - create the BEAMSCOOTER that carries the players away after the Apocalypse
 * fight (Apocalypse_StateVanish), through its class's New (slot +$0E).
 *
 * x, y: position (8 fraction bits). No placement entry. */
#include "xmen2.h"

void BeamScooter_Spawn(long x, long y)
{
	(*CLASS(Tbl_BeamScooter_vtable)->method[2])(0, CLASS(Tbl_BeamScooter_vtable), x, y, 0);	/* New */
}
