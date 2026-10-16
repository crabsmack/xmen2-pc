/* SavageB1_InstallHooks - Jungle 2's hooks: the hazard water's draw hook (Water_DrawHook) and the
 * frame hook SavageB1_FrameHook. */
#include "xmen2.h"

void SavageB1_InstallHooks(void)
{
	g_SceneDrawHook = FN_DATA(Water_DrawHook);
	g_SceneFrameHook = SavageB1_FrameHook;
}
