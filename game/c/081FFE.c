/* CloneA2_StartBgPalTimer - create and start the background palette timer (CloneA2_BgPalTimer,
 * every frame, its own Timer as argument), band 0 shown; with no free timer, nothing. */
#include "xmen2.h"

#define g_BgPalBand	(*(s16 *)g_SceneVar3B02)	/* scene variable: background palette shown, 0-3 */

void CloneA2_StartBgPalTimer(void)
{
	register Timer *t;

	if ((t = Timer_Create(CloneA2_BgPalTimer, 1, 0, 0)) != 0) {
		t->arg = t;
		Timer_Start(t);
		g_BgPalBand = 0;
	}
}
