/* ExplodeFx_BounceGroupAim - send the three puffs of a Bounce group away from the explosion's
 * centre: vx = offset x / 128, vy = offset y / 256 (in the group's 1/8-pixel units).
 *
 * g: the group.  cx, cy: the centre (8 fraction bits).
 * The two register pointers to vx and vy are set up and never used (the ROM keeps them). */
#include "xmen2.h"

void ExplodeFx_BounceGroupAim(FxGroup *g, register long cx, register long cy)
{
	register s8 *vx = g->vx;
	register s8 *vy = g->vy;
	register short i;

	for (i = 0; i < 3; i++) {
		g->vx[i] = ((g->x[i] << 5) - cx) >> 7;
		g->vy[i] = ((g->y[i] << 5) - cy) >> 8;
	}
}
