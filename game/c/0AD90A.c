/* SavageA2_InstallFrameHook - SavageA2's per-frame hook: SavageA2_FrameHook. */
#include "xmen2.h"

void SavageA2_InstallFrameHook(void)
{
	g_SceneFrameHook = SavageA2_FrameHook;
}
