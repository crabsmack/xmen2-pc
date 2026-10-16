/* Pause_Loop - the pause: frame after frame run the pause-screen input (Pause_HandleInput) until
 * it ends the pause (g_PauseExit). The pads' latched directions are dropped first, so a held
 * direction doesn't count as a press. */
#include "xmen2.h"

void Pause_Loop(void)
{
	g_Pad1Latch = g_Pad1Latch & ~0xF0;
	g_Pad2Latch = g_Pad2Latch & ~0xF0;
	do {
		g_FrameTick = 0;
		g_DmaFlushReq = 1;
		Pause_HandleInput();
		VDP_WaitVBlank();
	} while (!g_PauseExit);
}
