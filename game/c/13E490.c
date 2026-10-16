/* ExplodeFx_BounceGroupInit - a Bounce group of three puffs at (x, y) (1/8 pixels), still, each
 * at frame 0 with 15 frames left; one released.
 *
 * g: the group.  x, y: position (8 fraction bits). */
#include "xmen2.h"

void ExplodeFx_BounceGroupInit(register FxGroup *g, register long x, register long y)
{
	register short i;

	for (i = 0; i < 3; i++) {
		g->x[i] = x >> 5;
		g->y[i] = y >> 5;
		g->vx[i] = g->vy[i] = 0;
		g->anim[i] = 0xF;
	}
	g->count = 1;
}
