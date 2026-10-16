/* CloneA5_SegHook15 - floor-segment type hook for type $15 (CloneA5_RunScene): solid for standing
 * on (mode 3) only while BROODBOSS flag bit 7 is set.
 *
 * mode: what the segment code asks (3: is it solid for standing on).
 * Returns 1 or 0. */
#include "xmen2.h"

#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $40 wall smashed */

short CloneA5_SegHook15(short mode)
{
	if (mode == 3 && (g_BroodBossFlags & 0x80))
		return 1;
	else
		return 0;
}
