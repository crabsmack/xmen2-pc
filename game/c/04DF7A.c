/* ApocBomb_AccelTimer - the bomb-speed timer's function (ApocBomb_StartAccelTimer): the bombs
 * get faster by g_ApocBombAccel per tick, while below $280.
 *
 * (Called as func(arg, timer); both are unused.) */
#include "xmen2.h"

void ApocBomb_AccelTimer(void)
{
	if (g_ApocBombSpeed < 0x280)
		g_ApocBombSpeed += g_ApocBombAccel;
}
