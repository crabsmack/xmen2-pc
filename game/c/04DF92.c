/* ApocBomb_StartAccelTimer - create and start the bomb-speed timer (ApocBomb_AccelTimer, every
 * frame, its own Timer as argument) with speed and increase 0; with no free timer, none
 * (g_ApocBombTimer 0). */
#include "xmen2.h"

#define g_ApocBombTimer	(*(PTR(Timer) *)g_SceneVar3B20)	/* scene variable: the bomb-speed timer */

void ApocBomb_StartAccelTimer(void)
{
	register Timer *t;

	if ((t = Timer_Create(ApocBomb_AccelTimer, 1, 0, 0)) != 0) {
		t->arg = t;
		Timer_Start(t);
		g_ApocBombTimer = t;
		g_ApocBombSpeed = 0;
		g_ApocBombAccel = 0;
	} else
		g_ApocBombTimer = 0;
}
