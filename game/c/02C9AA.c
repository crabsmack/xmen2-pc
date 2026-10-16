/* AvalonA3_FrameHook - the Exodus arena's per-frame hook (g_SceneFrameHook): count the frame and
 * AvalonA3's countdown (both twice in two-player mode, g_TwoPlayerStep), animate the four
 * effect sprites, and step the psychic flash (AvalonA3_PsiFlashStep) while one runs and no
 * palette fade does.
 *
 * Effect sprite n (scene sprite n) shows a new frame every 5 frames: its step counts 2, 1, 0
 * and round again, frame step + 2 (sprites 1 and 3) or step + 6 (2 and 4), uploaded at once.
 * Each sprite's step is the word just before its delay in g_AvalonA3SprDelays (unnamed in the
 * disassembly: -$3CBE, -$3CC2, -$3CC6, -$3CCA; the macros below reach it from the delays). */
#include "xmen2.h"

#define g_A3Countdown	(*(s16 *)g_SceneVar3B06)	/* scene variable: counted down every frame (nothing reads it) */

#define SPR_DELAY(n)	g_AvalonA3SprAnim[4 - (n)].delay	/* frames to sprite n's next frame */
#define SPR_STEP(n)	g_AvalonA3SprAnim[4 - (n)].step	/* sprite n's animation step (0-2) */

void AvalonA3_FrameHook(void)
{
	g_FrameCounter++;
	g_A3Countdown--;
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		g_A3Countdown--;
	}
	if (!SPR_DELAY(1)) {
		SPR_DELAY(1) = 5;
		if (!SPR_STEP(1))
			SPR_STEP(1) = 3;
		--SPR_STEP(1);
		Spr_SetFrame(&g_SceneSprites[1], SPR_STEP(1) + 2);
		Spr_UploadFrame(&g_SceneSprites[1], 0);
	}
	--SPR_DELAY(1);
	if (!SPR_DELAY(2)) {
		SPR_DELAY(2) = 5;
		if (!SPR_STEP(2))
			SPR_STEP(2) = 3;
		--SPR_STEP(2);
		Spr_SetFrame(&g_SceneSprites[2], SPR_STEP(2) + 6);
		Spr_UploadFrame(&g_SceneSprites[2], 0);
	}
	--SPR_DELAY(2);
	if (!SPR_DELAY(3)) {
		SPR_DELAY(3) = 5;
		if (!SPR_STEP(3))
			SPR_STEP(3) = 3;
		--SPR_STEP(3);
		Spr_SetFrame(&g_SceneSprites[3], SPR_STEP(3) + 2);
		Spr_UploadFrame(&g_SceneSprites[3], 0);
	}
	--SPR_DELAY(3);
	if (!SPR_DELAY(4)) {
		SPR_DELAY(4) = 5;
		if (!SPR_STEP(4))
			SPR_STEP(4) = 3;
		--SPR_STEP(4);
		Spr_SetFrame(&g_SceneSprites[4], SPR_STEP(4) + 6);
		Spr_UploadFrame(&g_SceneSprites[4], 0);
	}
	--SPR_DELAY(4);
	if (!g_PalFadeActive && g_PsiFlashTimer)
		AvalonA3_PsiFlashStep();
}
