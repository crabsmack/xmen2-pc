/* SentinelsA1_InstallHooks - set up the Coolant Core after loading: the frame hook
 * SentinelsA1_FrameHook, its sprite sets, the zap lights, the palette cycles, the players at
 * their start (Players_PlaceAtStart, all arguments 0); each player present and alive gets sprite
 * flag bit 2 and its draw-list node goes to priority 1 at its layer (DrawList_SetPriority;
 * flags6 bit 3 passed on); the exit closed (its palette cycles off). */
#include "xmen2.h"

void SentinelsA1_InstallHooks(void)
{
	g_SceneFrameHook = SentinelsA1_FrameHook;
	SentinelsA1_LoadResources();
	SentinelsA1_InitZapLights();
	SentinelsA1_ResetPalCycles();
	Players_PlaceAtStart(0, 0, 0, 0);
	if (IS_ALIVE(&g_Player1)) {
		g_Player1.spr.flags |= 4;
		DrawList_SetPriority(g_Player1.drawNode, 1, g_Player1.layer, g_Player1.flags6 & 8);
	}
	if (IS_ALIVE(&g_Player2)) {
		g_Player2.spr.flags |= 4;
		DrawList_SetPriority(g_Player2.drawNode, 1, g_Player2.layer, g_Player2.flags6 & 8);
	}
	g_SentinelsA1ExitOpen = 0;
}
