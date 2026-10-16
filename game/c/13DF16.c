/* ExplodeFx_TrailSetHead - set the motion of a Trail's head puff and restart its step count.
 *
 * p: the head puff (puff 0).  x, y: its position (unused: ExplodeFx_TrailPuffInit set it).
 * vx, vy, ax, ay: its speed and acceleration. */
#include "xmen2.h"

void ExplodeFx_TrailSetHead(register FxPuff *p, long x, long y, short vx, short vy, short ax,
	short ay)
{
	p->vx = vx;
	p->vy = vy;
	p->ax = ax;
	p->ay = ay;
	p->anim = 0;
}
