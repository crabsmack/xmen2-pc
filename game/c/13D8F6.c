/* ExplodeFx_Free - end a death effect: stop and free its timer, take it off the draw list, and
 * leave a dead object behind (flags 0, its index back at +$15).
 *
 * fx: the effect (any kind: the Burst/Trail/Bounce/Chain updates call it when nothing is left). */
#include "xmen2.h"

void ExplodeFx_Free(register ExplodeFx *fx)
{
	if (fx->timer)
		Timer_Stop(fx->timer);
	if (fx->timer) {
		Timer_Free(fx->timer);
		fx->timer = 0;
	}
	if (fx->drawNode > -1) {
		DrawList_Remove(fx->drawNode);
		fx->drawNode = -1;
	}
	fx->flags = 0;
	((Actor *)fx)->index = fx->index;
}
