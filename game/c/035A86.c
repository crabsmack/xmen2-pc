/* AvalonA4_InstallHooks - the Throne Room's hooks: per-frame AvalonA4_FrameHook, map tile hooks
 * (AvalonA4_InstallTileHooks) and floor-segment event hooks (AvalonA4_InstallEventHooks). */
#include "xmen2.h"

void AvalonA4_InstallHooks(void)
{
	g_SceneFrameHook = AvalonA4_FrameHook;
	AvalonA4_InstallTileHooks();
	AvalonA4_InstallEventHooks();
}
