/* CloneA5_SegHook16 - floor-segment type hook for type $16 (CloneA5_RunScene; XClone_StartMakerMode
 * puts it on type $1A): always solid.
 *
 * Returns 1. Type $16 segments are the boss arena's hurting floor that BroodBossDamage_Init takes
 * over. */
#include "xmen2.h"

short CloneA5_SegHook16(void)
{
	return 1;
}
