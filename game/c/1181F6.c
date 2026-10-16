/* Explosion_Free - end an explosion puff: free its timer (if any) and the slot.
 *
 * e: the slot. */
#include "xmen2.h"

void Explosion_Free(register Explosion *e)
{
	if (e->timer)
		Timer_Free(e->timer);
	e->timer = 0;
}
