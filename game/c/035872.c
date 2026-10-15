/* AvalonA4_ClearCameraZone - leave camera zone n (0-5): drop its bit from the zone mask and
 * free the camera's axes again (CAMERA Cam_UnlockAxes).
 *
 * n: the zone (MagnetoBoss_StateBurstIn 0, _StateZone1 1, _StateZone2 2...).
 * Bug kept: the mask is and-ed with !bit (logical not: 0 for every zone) where ~bit was meant,
 * so any zone clears the whole mask. The switch cases are the fragments
 * AvalonA4_ClearCameraZone_case0 (zones 0-2), _case3, _case4, _case5; _apply is the rest. A zone
 * outside 0-5 uses whatever bit held (d7; !bit is then 0 unless d7's word was 0). */
#include "xmen2.h"

#define g_A4CamZones	(*(s16 *)g_SceneVar3B02)	/* scene variable: bit n = camera zone n held (AvalonA4_SetCameraZone) */

void AvalonA4_ClearCameraZone(short n)
{
	short bit;

	switch (n) {
	case 0: bit = 1; break;
	case 1: bit = 2; break;
	case 2: bit = 4; break;
	case 3: bit = 8; break;
	case 4: bit = 0x10; break;
	case 5: bit = 0x20; break;
	}
	g_A4CamZones &= (short)!bit;
	(*g_Camera->cls->method[13])(g_Camera);		/* +$3A Cam_UnlockAxes */
}
