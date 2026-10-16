/* SentinelsB2_InstallHooks - set up the Sentinels escape after loading: the frame hook
 * SentinelsB2_FrameHook, the warning light (SentinelsD1_InitLight), the HUD on, the countdown
 * digits, the alarm at step 0, the players at (2480, 324) and (2470, 314) (Players_PlaceAtStart),
 * the escape countdown at $546 (135 seconds) not yet shown, and the camera shaking
 * (Tbl_CoreShieldsQuake). */
#include "xmen2.h"

#define g_EscapeTime	(*(s16 *)g_SceneVar3B02)	/* scene variable: escape countdown, tenths of a second */
#define g_EscapeMode	(*(s16 *)g_SceneVar3B08)	/* scene variable: 0 counting, 1 showing, 2 white fade, 3 black fade */

void SentinelsB2_InstallHooks(void)
{
	g_SceneFrameHook = SentinelsB2_FrameHook;
	SentinelsD1_InitLight();
	g_HudOn = 1;
	SentinelsB2_LoadDigits();
	SentinelsB2_ResetAlarm();
	Players_PlaceAtStart(0x9B0, 0x144, 0x9A6, 0x13A);
	g_EscapeTime = 0x546;
	g_EscapeMode = 0;
	g_CamShake = CODEPTR(Tbl_CoreShieldsQuake);
}
