/* XNpc_KnockbackDist - how far (pixels) a hit of a strength knocks an XNPC back: its stats
 * record's knockback distance for strength 1, 2, 4 or 8; 0 for any other strength.
 *
 * self: the object.  strength: the hit's damage (XNpc_HurtReact, XNpc_DeathReact). */
#include "xmen2.h"

short XNpc_KnockbackDist(XNpc *self, char strength)
{
	switch (strength) {
	case 1:
		return self->stats->knockDist[0];
	case 2:
		return self->stats->knockDist[1];
	case 4:
		return self->stats->knockDist[2];
	case 8:
		return self->stats->knockDist[3];
	default:
		return 0;
	}
}
