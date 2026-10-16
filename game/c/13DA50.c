/* ExplodeFx_BurstPuffInit - a live Burst puff at (x, y), still, at frame 0 (6 frames left).
 *
 * p: the puff.  x, y: position (8 fraction bits). */
#include "xmen2.h"

void ExplodeFx_BurstPuffInit(register FxPuff *p, long x, long y)
{
	p->x = x;
	p->y = y;
	p->vx = 0;
	p->vy = 0;
	p->ax = 0;
	p->ay = 0;
	p->anim = 6;
	p->type = 1;
}
