/* SentinelsB2_ResetAlarm - restart the Sentinels escape's alarm palette (SentinelsB2_AlarmPalette)
 * at step 0, and clear two other scene variables nothing else reads there. */
#include "xmen2.h"

#define g_AlarmStep	(*(s16 *)g_SceneVar3B38)	/* scene variable: step of the 25-step alarm flash */
#define g_B2Unused48	(*(s16 *)g_SceneVar3B48)	/* scene variable: cleared here, never read in the scene */
#define g_B2Unused40	(*(s16 *)g_SceneVar3B40)	/* scene variable: cleared here, never read in the scene */

void SentinelsB2_ResetAlarm(void)
{
	g_AlarmStep = 0;
	g_B2Unused48 = 0;
	g_B2Unused40 = 0;
}
