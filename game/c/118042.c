/* Explosion_ResetAll - free every explosion slot (g_Explosions, 10) without stopping its timer:
 * no timer, not hurting. Run at each scene start (the timers are reset there too). */
#include "xmen2.h"

void Explosion_ResetAll(void)
{
	register Explosion *slots = g_Explosions;
	register Explosion *e = &slots[0];
	register short i;

	for (i = 0; i < 10; i++) {
		e->timer = 0;
		e->hurts = 0;
		e++;
	}
}
