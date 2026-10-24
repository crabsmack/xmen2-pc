/* ApocBomb_SetSpeed - APOCBOMB class settings for a bombing round (Apocalypse_ActStartBombs):
 * the period of the bomb-speed timer (if it exists), the bombs' speed and its increase per
 * timer tick.
 *
 * period: frames between ApocBomb_AccelTimer ticks.  speed: g_ApocBombSpeed.
 * accel: g_ApocBombAccel. */
#include "xmen2.h"

#define g_ApocBombTimer	(*(PTR(Timer) *)&g_SceneVar3B20)	/* scene variable: the bomb-speed timer */

void ApocBomb_SetSpeed(short period, short speed, short accel)
{
	register Timer *t;

	if ((t = g_ApocBombTimer) != 0)
		t->period = period;
	g_ApocBombSpeed = speed;
	g_ApocBombAccel = accel;
}
