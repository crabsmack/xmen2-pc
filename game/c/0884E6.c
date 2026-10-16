/* CloneA5_FrameHook - the boss arena's scene hook, once per frame. The first time (g_BroodBossFlags
 * bit 5 still clear): the arena's sprite definitions again with VRAM re-packed (Spr_ReloadDefs of
 * CloneA5_LoadResources) and the boss (CloneA5_SpawnBoss). Then: count the frame, animate the
 * shock sprites, and while the boss lives cycle the arena's palette (not during a fade) with one
 * step every health-bar frames: the less health, the faster. Twice in two-player step mode. */
#include "xmen2.h"

#define g_BroodBossFlags	(*(s16 *)g_SceneVar3B0C)	/* scene variable: 1 fight on, 2 floor shock, $10 lower the body, $20 resources loaded, $40 wall smashed */
#define g_BroodPalTable		(*(PTR(PalCycleSeq) *)g_SceneVar3B18)	/* scene variable (CloneA5): the arena's palette cycle table (a RAM copy of Tbl_CloneA5PalCycle, CloneA5_RunScene) */

void CloneA5_FrameHook(void)
{
	if (!(g_BroodBossFlags & 0x20)) {
		g_BroodBossFlags |= 0x20;
		Spr_ReloadDefs(CloneA5_LoadResources, 1);
		CloneA5_SpawnBoss();
	}
	g_FrameCounter++;
	CloneA5_AnimShockSprites();
	if (g_BroodBossHealthBar)
		g_BroodPalTable->delay = g_BroodBossHealthBar;
	if (!g_PalFadeActive && g_BroodBossHealthBar)
		Pal_CycleStep(&g_CloneA5PalTimer, &g_CloneA5PalColour, &g_CloneA5PalSeq,
			g_BroodPalTable, 1);
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		CloneA5_AnimShockSprites();
		if (!g_PalFadeActive && g_BroodBossHealthBar)
			Pal_CycleStep(&g_CloneA5PalTimer, &g_CloneA5PalColour, &g_CloneA5PalSeq,
				g_BroodPalTable, 1);
	}
}
