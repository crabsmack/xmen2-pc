/* AvalonB2_FrameHook - the Inner Shell's per-frame scene hook (g_SceneFrameHook): g_FrameCounter
 * advances (by 2 with g_TwoPlayerStep), then the palette cycles, AvalonB1's
 * (AvalonB1_CyclePalettes). */
#include "xmen2.h"

void AvalonB2_FrameHook(void)
{
	g_FrameCounter++;
	if (g_TwoPlayerStep)
		g_FrameCounter++;
	AvalonB1_CyclePalettes();
}
