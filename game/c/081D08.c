/* CloneA2_FrameHook - the CloneA2 scene hook, once per frame: count the frame; start the
 * earthquake (CloneA2_StartQuake) once the quake point of the current camera zone is inside
 * the 320x224 view; run the palette flicker (unless a fade runs or it is off), twice in
 * two-player step mode. */
#include "xmen2.h"

#define g_CloneA2PalOff	(*(s16 *)g_SceneVar3B04)	/* scene variable: non-zero: no flicker, no background palette swaps */
#define g_CloneA2Zone	(*(s16 *)g_SceneVar3B06)	/* scene variable: current camera zone (CloneA2_SetCameraZone) */
#define g_QuakeDone	(*(s16 *)g_SceneVar3B08)	/* scene variable: the zone's quake has started */

void CloneA2_FrameHook(void)
{
	register CamZone *zones;
	register short qx;
	register short qy;

	g_FrameCounter++;
	if (!g_QuakeDone) {
		zones = (CamZone *)CODEPTR(Tbl_CloneA2CameraZones);
		qx = zones[g_CloneA2Zone].quakeX;
		qy = zones[g_CloneA2Zone].quakeY;
		/* the game's bound (ACT_, phase 6 R8): the quake moves the camera */
		if (qx && qy && qx > ACT_L(g_CameraX) && qx < g_CameraX + ACT_R(320) && qy > g_CameraY && qy < g_CameraY + 224)
			CloneA2_StartQuake();
	}
	if (!g_PalFadeActive && !g_CloneA2PalOff)
		CloneA2_FlickerUpdate();
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		if (!g_PalFadeActive && !g_CloneA2PalOff)
			CloneA2_FlickerUpdate();
	}
}
