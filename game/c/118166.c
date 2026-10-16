/* Explosion_SpawnSetTimerWord - start a harmless explosion puff centred on (x, y) that waits
 * before it shows (chains of puffs: 0, 4, 8... frames).
 *
 * x, y:     centre (8 fraction bits).
 * priority: non-zero: the sprite has high priority.
 * delay:    its timer's state: frames before the first step (Timer.state).
 * Returns 0 if none started; when one did, the function falls off the end with the slot still
 * in d0 (from the test of Explosion_SpawnEx's result). */
#include "xmen2.h"

Explosion *Explosion_SpawnSetTimerWord(long x, long y, unsigned char priority, short delay)
{
	register Explosion *e;

	if (e = Explosion_SpawnEx(x, y, priority, 0)) {
		if (e->timer)
			e->timer->state = delay;
		else
			return 0;
	} else
		return 0;
#ifndef XMEN2_MPW
	return e;	/* what d0 holds on the 68000 */
#endif
}
