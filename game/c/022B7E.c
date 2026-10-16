/* AvalonA2_InstallHooks - the Falling Room's hooks: the background parallax
 * (Tbl_AvalonA2Parallax, Scroll_SetParallax), the frame hook AvalonA2_FrameHook, the tile hooks
 * and the event hooks. */
#include "xmen2.h"

void AvalonA2_InstallHooks(void)
{
	Scroll_SetParallax(CODEPTR(Tbl_AvalonA2Parallax));
	g_SceneFrameHook = AvalonA2_FrameHook;
	AvalonA2_InstallTileHooks();
	AvalonA2_InstallEventHooks();
}
