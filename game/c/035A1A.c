/* AvalonA4_EventZoneFree5 - floor-segment type $1A hook of the Throne Room (an event segment,
 * AvalonA4_InstallEventHooks): for message 3, 1 while camera zone 5 is free (bit 5 of the zone
 * mask clear: AvalonA4_SetCameraZone / _ClearCameraZone), else 0.
 *
 * msg: the hook's message (the other arguments are unused). */
#include "xmen2.h"

#define g_A4CamZones	(*(s16 *)g_SceneVar3B02)	/* scene variable: bit n = camera zone n held */

short AvalonA4_EventZoneFree5(short msg)
{
	if (msg == 3) {
		if (g_A4CamZones & 0x20)
			return 0;
		return 1;
	}
	return 0;
}
