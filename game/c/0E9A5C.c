/* Hologram_SetPose - show the hologram in a pose (the scene variable Hologram_Draw reads).
 *
 * pose: 0-4 standing/raising/lowering, 5-6 firing, $1B hidden (Hologram_Draw draws nothing).
 * Hiding it (from another pose) arms the flicker again with no sprite slot
 * (g_HoloFlickerArmed 1, g_HoloFlickerLinks[1] 0: Hologram_VBlankFlicker). */
#include "xmen2.h"

#define g_HologramPose	(*(s32 *)g_SceneVar3B14)	/* scene variable: pose drawn */

void Hologram_SetPose(register short pose)
{
	if (pose == 0x1B && g_HologramPose != 0x1B) {
		g_HoloFlickerArmed = 1;
		g_HoloFlickerLinks[1] = 0;
	}
	g_HologramPose = pose;
}
