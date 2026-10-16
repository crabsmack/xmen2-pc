/* Cerebro_FrameHook - the Cerebro screens' per-frame scene hook (g_SceneFrameHook, Cerebro_Run):
 * g_FrameCounter advances (by 2 with g_TwoPlayerStep), the pads are read (Pad_Update), a text
 * wait (text state 2) ends on a button press (A, B, C, Start on either pad), and the screen's
 * palettes animate (Cerebro_AnimatePalettes). */
#include "xmen2.h"

#define g_TextState	(*(s16 *)g_SceneVar3B02)	/* scene variable: Cerebro text state (2: waiting, a button ends it) */

void Cerebro_FrameHook(void)
{
	g_FrameCounter++;
	if (g_TwoPlayerStep)
		g_FrameCounter++;
	Pad_Update();
	if (g_TextState == 2 && (g_Pad1Pressed | g_Pad2Pressed) & 0xF0)
		g_TextState = 0;
	Cerebro_AnimatePalettes();
}
