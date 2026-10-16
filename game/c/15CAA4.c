/* Pause_Check - the Start press of a level (g_PauseHandler, called by Level_FrameLoop): a dead
 * player with lives left who pressed Start comes back (Player1_StartLife / Player2_StartLife);
 * otherwise the game pauses: sound paused, the PAUSE text shown in front of the players
 * (Pause_LoadSprite, Pause_ShowText, Pause_LinkSprites), the pause run (Pause_Loop), then all
 * of it undone. Nothing while the HUD sprite isn't up, during a fade or with both players dead. */
#include "xmen2.h"

void Pause_Check(void)
{
	g_PauseCheatPtr = 0;
	if (g_SpriteTable[0].link && !g_PalFadeActive && (g_Player1.health > 0 || g_Player2.health > 0)) {
		if ((g_Pad1Pressed & 0x80) && g_Player1.health <= 0) {
			if (g_P1Lives)
				Player1_StartLife();
		} else if ((g_Pad2Pressed & 0x80) && g_Player2.health <= 0) {
			if (g_P2Lives)
				Player2_StartLife();
		} else {
			g_PauseExit = 0;
			g_Paused = 1;
			g_Pad1Latch = g_Pad1Latch & ~0x80;
			Snd_Pause();
			Pause_LoadSprite();
			Pause_ShowText();
			Pause_LinkSprites();
			Pause_Loop();
			Pause_RestoreLink();
			Snd_Resume();
		}
	}
}
