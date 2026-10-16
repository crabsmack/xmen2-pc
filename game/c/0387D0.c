/* BaniMazaA1_InstallTileHooks - the Moving Blocks' map tile hooks: 1 the switch
 * (BaniMazaA1_SwitchTileHook), 0 and 2-7 none; tile hooks on (g_TileHooksOn), no switch
 * cooldown. */
#include "xmen2.h"

void BaniMazaA1_InstallTileHooks(void)
{
	g_TileHooksOn = 1;
	g_TileHooks[0] = 0;
	g_TileHooks[1] = BaniMazaA1_SwitchTileHook;
	g_TileHooks[2] = 0;
	g_TileHooks[3] = 0;
	g_TileHooks[4] = 0;
	g_TileHooks[5] = 0;
	g_TileHooks[6] = 0;
	g_TileHooks[7] = 0;
	g_BmA1SwitchTimer = 0;
}
