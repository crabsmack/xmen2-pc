/* FarAcolyte_StateDie - FARACOLYTE hurt state (+$60): an explosion at the centre of its body
 * box, then the medium burst death (ExplodeFx_BurstMedium).
 *
 * self: the acolyte (never runs in the corpus). */
#include "xmen2.h"

void FarAcolyte_StateDie(register FarAcolyte *self)
{
	long cx, cy;

	cx = (long)(self->boxRight + self->boxLeft) << 7;
	cy = (long)(self->boxTop + self->boxBottom) << 7;
	Explosion_Spawn(cx, cy, 0);
	ExplodeFx_BurstMedium(self);
}
