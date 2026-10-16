/* ExplodeFx_BurstPuffAim - send a Burst puff away from the explosion's centre: speed 1/32 of its
 * offset, acceleration 1/1024 of the x offset, gravity 12.
 *
 * p: the puff.  cx, cy: the centre (8 fraction bits). */
#include "xmen2.h"

void ExplodeFx_BurstPuffAim(register FxPuff *p, register long cx, long cy)
{
	p->vx = (p->x - cx) >> 5;
	p->vy = (p->y - cy) >> 5;
	p->ax = (p->x - cx) >> 10;
	p->ay = 12;
}
