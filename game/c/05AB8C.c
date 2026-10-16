/* BaniMazaB2_FrameHook - BaniMazaB2's per-frame scene hook (g_SceneFrameHook): g_FrameCounter
 * advances, by 2 with g_TwoPlayerStep. */
#include "xmen2.h"

void BaniMazaB2_FrameHook(void)
{
	g_FrameCounter++;
	if (g_TwoPlayerStep)
		g_FrameCounter++;
}
