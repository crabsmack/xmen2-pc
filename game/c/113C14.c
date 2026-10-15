/* XNpc_Explode - XNPC Explode (class slot +$7E): the explosion sound ($38), then the death
 * effect its stats record's byte 2 (health) picks.
 *
 * self: the object.
 * 2 ExplodeFx_BurstMedium, 4 ExplodeFx_TrailArc, 8 ExplodeFx_Bounce, anything else (1, the
 * usual) ExplodeFx_BurstSmall. Each effect kills the object. */
#include "xmen2.h"

void XNpc_Explode(XNpc *self)
{
	Snd_Stop(0x38);
	Snd_Stop(0x38);
	Snd_Play(0x38);
	switch (self->stats->health) {
	case 8:
		ExplodeFx_Bounce(self);
		break;
	case 4:
		ExplodeFx_TrailArc(self);
		break;
	case 2:
		ExplodeFx_BurstMedium(self);
		break;
	case 1:
	default:
		ExplodeFx_BurstSmall(self);
		break;
	}
}
