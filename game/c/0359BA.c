/* AvalonA4_EventZoneFree2 - floor-segment type $17 hook of the Throne Room (an event segment,
 * AvalonA4_InstallEventHooks): for message 3, 1 while camera zone 2 is free (bit 2 of the zone
 * mask clear: AvalonA4_SetCameraZone / _ClearCameraZone), else 0.
 *
 * msg: the hook's message (the other arguments are unused). */
#include "xmen2.h"

#define g_A4CamZones	(*(s16 *)g_SceneVar3B02)	/* scene variable: bit n = camera zone n held */

short AvalonA4_EventZoneFree2(short msg)
{
	if (msg == 3) {
		if (g_A4CamZones & 4)
			return 0;
		return 1;
	}
	return 0;
}
