/* AvalonA2_InstallTileHooks - the Falling Room's map tile hooks: only hook type 2,
 * AvalonA2_ToggleTileHook; hooks on. */
#include "xmen2.h"

void AvalonA2_InstallTileHooks(void)
{
	g_TileHooks[0] = 0;
	g_TileHooks[1] = 0;
	g_TileHooks[2] = AvalonA2_ToggleTileHook;
	g_TileHooks[3] = 0;
	g_TileHooks[4] = 0;
	g_TileHooks[5] = 0;
	g_TileHooks[6] = 0;
	g_TileHooks[7] = 0;
	g_TileHooksOn = 1;
}
