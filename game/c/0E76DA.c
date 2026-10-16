/* SentinelsA1_StepZapLights - every 4 frames, step the Coolant Core's zap lights (scene sprites
 * 0-4): each running light shows its frame now (Spr_SetFrame, Spr_UploadFrame) and moves on, up
 * to frame 7, then goes idle (-1); an idle light 1-4 starts (frame 0) when the light before it
 * is at frame 4. Light 0 is started by CoreShields_FireZap.
 *
 * The 4-frame timer (a long scene variable) is counted down by SentinelsA1_FrameHook. The
 * sprite pointer is stepped once more after the last light. */
#include "xmen2.h"

#define g_ZapLight0	(*(s16 *)g_SceneVar3B02)	/* scene variable: frame of zap light 0 (-1 idle) */
#define g_ZapLight1	(*(s16 *)g_SceneVar3B04)	/* scene variable: frame of zap light 1 */
#define g_ZapLight2	(*(s16 *)g_SceneVar3B06)	/* scene variable: frame of zap light 2 */
#define g_ZapLight3	(*(s16 *)g_SceneVar3B08)	/* scene variable: frame of zap light 3 */
#define g_ZapLight4	(*(s16 *)g_SceneVar3B0A)	/* scene variable: frame of zap light 4 */
#define g_ZapTimer	(*(s32 *)g_SceneVar3B18)	/* scene variable: frames to the lights' next step */

void SentinelsA1_StepZapLights(void)
{
	register SprInst *spr = g_SceneSprites;

	if (g_ZapTimer <= 0) {
		g_ZapTimer = 4;
		if (g_ZapLight0 >= 0) {
			Spr_SetFrame(spr, g_ZapLight0);
			Spr_UploadFrame(spr, 0);
			g_ZapLight0++;
			if (g_ZapLight0 > 7)
				g_ZapLight0 = -1;
		}
		spr++;
		if (g_ZapLight1 >= 0) {
			Spr_SetFrame(spr, g_ZapLight1);
			Spr_UploadFrame(spr, 0);
			g_ZapLight1++;
			if (g_ZapLight1 > 7)
				g_ZapLight1 = -1;
		} else if (g_ZapLight0 == 4)
			g_ZapLight1 = 0;
		spr++;
		if (g_ZapLight2 >= 0) {
			Spr_SetFrame(spr, g_ZapLight2);
			Spr_UploadFrame(spr, 0);
			g_ZapLight2++;
			if (g_ZapLight2 > 7)
				g_ZapLight2 = -1;
		} else if (g_ZapLight1 == 4)
			g_ZapLight2 = 0;
		spr++;
		if (g_ZapLight3 >= 0) {
			Spr_SetFrame(spr, g_ZapLight3);
			Spr_UploadFrame(spr, 0);
			g_ZapLight3++;
			if (g_ZapLight3 > 7)
				g_ZapLight3 = -1;
		} else if (g_ZapLight2 == 4)
			g_ZapLight3 = 0;
		spr++;
		if (g_ZapLight4 >= 0) {
			Spr_SetFrame(spr, g_ZapLight4);
			Spr_UploadFrame(spr, 0);
			g_ZapLight4++;
			if (g_ZapLight4 > 7)
				g_ZapLight4 = -1;
		} else if (g_ZapLight3 == 4)
			g_ZapLight4 = 0;
		spr++;
	}
}
