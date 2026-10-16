/* SavageA3_InstallFrameHook - the Perimeter's per-frame hook: SavageA3_FrameHook. */
#include "xmen2.h"

void SavageA3_InstallFrameHook(void)
{
	g_SceneFrameHook = SavageA3_FrameHook;
}
