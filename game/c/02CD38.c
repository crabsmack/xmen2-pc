/* AvalonA3_InstallHooks - the Exodus arena's per-frame hook: AvalonA3_FrameHook. */
#include "xmen2.h"

void AvalonA3_InstallHooks(void)
{
	g_SceneFrameHook = AvalonA3_FrameHook;
}
