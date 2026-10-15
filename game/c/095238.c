/* XClone_ClassInit - XCLONE per-level class setup (class slot +$5E): no clone killed yet, no
 * reward pending. Loads no resources.
 *
 * Clears g_CloneMakerKills, g_CloneWaveKills (6 end the wave) and the reward timer
 * (g_SceneVar3B06), then calls the empty XClone_NopStatic. */
#include "xmen2.h"

#define g_CloneRewardTimer	(*(s16 *)g_SceneVar3B06)	/* scene variable: frames before the next reward */

void XClone_ClassInit(void)
{
	g_CloneMakerKills = 0;
	g_CloneWaveKills = 0;
	g_CloneRewardTimer = 0;
	XClone_NopStatic();
}
