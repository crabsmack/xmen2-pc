/* SavageC1_InstallHooks - Jungle 1's hooks: the hazard water's draw hook (Water_DrawHook) and the
 * frame hook SavageC1_FrameHook. */
#include "xmen2.h"

void SavageC1_InstallHooks(void)
{
	g_SceneDrawHook = FN_DATA(Water_DrawHook);
	g_SceneFrameHook = SavageC1_FrameHook;
}
