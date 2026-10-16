/* Demo_ReadInput - play back an attract demo's recorded input: once the level frame reaches the
 * next step, the next pad state becomes g_DemoPad (what Pad_Get gives player 1).
 *
 * g_DemoInput reads (frames, pad) byte pairs: a step comes the given number of frames after the
 * last one (g_DemoStepFrame). A pad of $FF ends the demo: no buttons, and the scene ends in 2
 * frames (Scene_EndAfter). */
#include "xmen2.h"

/* the input stream as bytes */
#define g_DemoBytes	(*(PTR(u8) *)&g_DemoInput)

void Demo_ReadInput(void)
{
	if (g_LevelFrame == (*g_DemoBytes & 0xFF) + g_DemoStepFrame) {
		g_DemoBytes++;
		g_DemoPad = (s8)*g_DemoBytes++;
		g_DemoStepFrame = g_LevelFrame;
		if (g_DemoPad == -1) {
			g_DemoPad = 0;
			Scene_EndAfter(2);
		}
	}
}
