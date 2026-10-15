/* SiberiaB1_FrameHook - the Blizzard's scene hook, once per frame (twice for the counters, the
 * palette cycles and the wind in two-player step mode): count the frame, count the wind step
 * and SiberiaB1's other countdown down to 0, the wind sprite, the wind, the palette cycles
 * (unless a fade runs).
 * In the opening (g_SiberiaIntro): it ends once player 1 is past x $360; when player 2 joins,
 * the opening music stops (sounds $27-$2F) and the stage music starts again for two players.
 * The power-on demo: when its timeout runs out the game goes to the logo and demo scene; any
 * button cancels the timeout (and with progress flag 1, sets progress flag 2). */
#include "xmen2.h"

#define g_WindTimer	(*(s16 *)g_SceneVar3B08)	/* scene variable: frames to the next wind step */
#define g_DemoTimeout	(*(s16 *)g_SceneVar3B0A)	/* scene variable: frames left of the power-on demo (0: none) */
#define g_SiberiaB1Count	(*(s16 *)g_SceneVar3B32)	/* scene variable: a countdown (nothing else reads it) */

void SiberiaB1_FrameHook(void)
{
	g_FrameCounter++;
	if (g_WindTimer > 0)
		g_WindTimer--;
	if (g_SiberiaB1Count > 0)
		g_SiberiaB1Count--;
	SiberiaB1_UpdateWindSprite();
	SiberiaB1_StepWind();
	if (!g_PalFadeActive)
		SiberiaB1_CyclePalettes();
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		if (g_WindTimer > 0)
			g_WindTimer--;
		if (g_SiberiaB1Count > 0)
			g_SiberiaB1Count--;
		if (!g_PalFadeActive)
			SiberiaB1_CyclePalettes();
		SiberiaB1_UpdateWindSprite();
		SiberiaB1_StepWind();
	}
	if (g_SiberiaIntro) {
		if (g_Player1.x >> 8 > 0x360)
			g_SiberiaIntro = 0;
		if (IS_ALIVE(&g_Player2)) {
			Snd_Stop(0x27);
			Snd_Stop(0x28);
			Snd_Stop(0x29);
			Snd_Stop(0x2B);
			Snd_Stop(0x2C);
			Snd_Stop(0x2D);
			Snd_Stop(0x2F);
			Music_SetPlayerParams();
			Music_PlayStage(0x27);
			g_SiberiaIntro = 0;
		}
	}
	if (g_DemoTimeout) {
		--g_DemoTimeout;
		if (!g_DemoTimeout) {
			g_SceneExitTimer = 1;
			g_SceneIndex = g_ScnLogoAndDemo;
			g_SceneIndexSet = 1;
		}
		if ((g_Pad1Raw | g_Pad2Raw) & 0xFF) {
			g_DemoTimeout = 0;
			if (g_ProgressFlags & 1)
				g_ProgressFlags |= 2;
		}
	}
}
