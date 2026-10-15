/* CloneA2_SetCameraZone - hold the CloneA2 camera in zone n of Tbl_CloneA2CameraZones: its limits
 * (and their targets) from the zone (maximum = right/bottom edge less the 320x224 view), and
 * the zone's quake armed again.
 *
 * zone: the zone. Called by CloneA2_StartCamera (zone 0) and Teleporter_StateArrive. */
#include "xmen2.h"

#define g_CloneA2Zone	(*(s16 *)g_SceneVar3B06)	/* scene variable: current camera zone */
#define g_QuakeDone	(*(s16 *)g_SceneVar3B08)	/* scene variable: the zone's quake has started */

void CloneA2_SetCameraZone(register short zone)
{
	register CamZone *zones = (CamZone *)CODEPTR(Tbl_CloneA2CameraZones);
	short left = zones[zone].left;
	register short top = zones[zone].top;
	register short right = zones[zone].right - 0x140;
	register short bottom = zones[zone].bottom - 0xE0;

	g_CamMinX = g_CamMinXTarget = left;
	g_CamMinY = g_CamMinYTarget = top;
	g_CamMaxX = g_CamMaxXTarget = right;
	g_CamMaxY = g_CamMaxYTarget = bottom;
	g_CloneA2Zone = zone;
	g_QuakeDone = 0;
}
