/* CloneA5_InitTileHooks - the boss arena's map tile hooks: the clone scenes' (XClone_InstallTileHooks).
 *
 * Returns what XClone_InstallTileHooks returned (CloneA5_RunScene stores it in g_TileHooksOn):
 * the original keeps it in a register variable it never uses and has no return statement; d0
 * still holds the value. */
#include "xmen2.h"

char CloneA5_InitTileHooks(void)
{
	register char on;

	on = (char)XClone_InstallTileHooks();
#ifndef XMEN2_MPW
	return on;	/* MPW: falls off the end with the value still in d0 */
#endif
}
