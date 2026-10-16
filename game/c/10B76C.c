/* SpaceA1_InstallTileHooks - the Space Elevator's map tile hooks: only hook type 2, the lift's
 * switch (SpaceA1_SwitchTileHit); hooks on. */
#include "xmen2.h"

void SpaceA1_InstallTileHooks(void)
{
	g_TileHooks[0] = 0;
	g_TileHooks[1] = 0;
	g_TileHooks[2] = SpaceA1_SwitchTileHit;
	g_TileHooks[3] = 0;
	g_TileHooks[4] = 0;
	g_TileHooks[5] = 0;
	g_TileHooks[6] = 0;
	g_TileHooks[7] = 0;
	g_TileHooksOn = 1;
}
