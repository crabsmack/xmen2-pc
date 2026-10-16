/* AvalonA2_ResetCounters - restart the Falling Room's palette effect (AvalonA2_DisabledFrameEffect,
 * which never runs): delay 1, step 0. */
#include "xmen2.h"

#define g_A2PalDelay	(*(s16 *)g_SceneVar3B32)	/* scene variable: frames before the palette effect's next step (only set) */
#define g_A2PalStep	(*(s16 *)g_SceneVar3B3A)	/* scene variable: step 0-3 of the palette effect */

void AvalonA2_ResetCounters(void)
{
	g_A2PalDelay = 1;
	g_A2PalStep = 0;
}
