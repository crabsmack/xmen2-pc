/* SavageA2_InstallTileHooks - SavageA2's map tile hooks: hook type 2 cracks breakable blocks
 * (SavageA2_TileHookCrack), type 3 is the perimeter door's (PerimeterDoor_TileHook), the others
 * none; hooks on. */
#include "xmen2.h"

void SavageA2_InstallTileHooks(void)
{
	g_TileHooks[0] = 0;
	g_TileHooks[1] = 0;
	g_TileHooks[2] = SavageA2_TileHookCrack;
	g_TileHooks[3] = PerimeterDoor_TileHook;
	g_TileHooks[4] = 0;
	g_TileHooks[5] = 0;
	g_TileHooks[6] = 0;
	g_TileHooks[7] = 0;
	g_TileHooksOn = 1;
}
