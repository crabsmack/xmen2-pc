/* CloneA1_InstallHooks - the CloneA1 hooks: the scene frame hook (CloneA1_FrameHook), its sprite
 * definitions, scene sprites and palette cycles; the players at their start (Players_PlaceAtStart,
 * all arguments 0), and each live player drawn with priority (sprite flag bit 2, draw-list
 * priority 1) in front of the factory's foreground rows. */
#include "xmen2.h"

void CloneA1_InstallHooks(void)
{
	g_SceneFrameHook = CloneA1_FrameHook;
	CloneA1_LoadResources();
	CloneA1_InitSprites();
	CloneA1_ResetPalCycle();
	Players_PlaceAtStart(0, 0, 0, 0);
	if (IS_ALIVE(&g_Player1)) {
		g_Player1.spr.flags |= 4;
		DrawList_SetPriority(g_Player1.drawNode, 1, g_Player1.layer, g_Player1.flags6 & 8);
	}
	if (IS_ALIVE(&g_Player2)) {
		g_Player2.spr.flags |= 4;
		DrawList_SetPriority(g_Player2.drawNode, 1, g_Player2.layer, g_Player2.flags6 & 8);
	}
#if defined(XMEN2_NATIVE)
	P34_SetLayer(1, 1);	/* players 3-4 in play too (R11) */
#endif
}
