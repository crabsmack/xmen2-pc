/* SiberiaB1_InstallHooks - the Blizzard's frame hook (SiberiaB1_FrameHook). With no player 1
 * yet (the power-on demo): seed the random generator from the V counter (read until two reads
 * agree, interrupts off) + g_AttractDemoNum, start player 1's life and a 1200-frame demo
 * timeout; else no timeout (also none with progress flag 2). Then the players at the start,
 * its sprite definitions, the ice tiles, the palette cycles and the wind. */
#include "xmen2.h"

#define g_DemoTimeout	(*(s16 *)g_SceneVar3B0A)	/* scene variable: frames left of the power-on demo (0: none) */

void SiberiaB1_InstallHooks(void)
{
	register short v;

	g_SceneFrameHook = SiberiaB1_FrameHook;
	if (!g_Player1.cls) {
		Irq_Disable();
		do
			v = VDP_HV_B;
		while (v != VDP_HV_B);
		Irq_Enable();
		Rand_Seed(v + g_AttractDemoNum);
		Player1_StartLife();
		g_DemoTimeout = 1200;
	} else
		g_DemoTimeout = 0;
	if (g_ProgressFlags & 2)
		g_DemoTimeout = 0;
	Players_PlaceAtStart(0, 0, 0, 0);
	SiberiaB1_LoadResources();
	SiberiaB1_InstallTileHooks();
	SiberiaB1_ResetPalCycles();
	SiberiaB1_InitWind();
}
