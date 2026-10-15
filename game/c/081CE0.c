/* CloneA2_StartQuake - start the CloneA2 earthquake: the camera plays Tbl_CloneA2QuakeScript,
 * rumble sound $91, and it is not started again in this camera zone (CloneA2_SetCameraZone
 * re-arms it). */
#include "xmen2.h"

#define g_QuakeDone	(*(s16 *)g_SceneVar3B08)	/* scene variable: the zone's quake has started */

void CloneA2_StartQuake(void)
{
	g_CamShake = CODEPTR(Tbl_CloneA2QuakeScript);
	Snd_Stop(0x91);
	Snd_Play(0x91);
	g_QuakeDone = 1;
}
