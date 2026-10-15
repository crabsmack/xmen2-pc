/* CloneA2_SetCameraLimitsToMap - let the CloneA2 camera go over the whole map: limits (and their
 * targets) 0 .. the map's size (g_LevelFgMap header, 8-px cells) less the 320x224 view.
 * Called by Teleporter_StateTeleport. */
#include "xmen2.h"

void CloneA2_SetCameraLimitsToMap(void)
{
	register short left = 0;
	register short top = 0;
	register short right = (((MapHeader *)g_LevelFgMap)->width << 3) - 0x140;
	register short bottom = (((MapHeader *)g_LevelFgMap)->height << 3) - 0xE0;

	g_CamMinX = g_CamMinXTarget = left;
	g_CamMinY = g_CamMinYTarget = top;
	g_CamMaxX = g_CamMaxXTarget = right;
	g_CamMaxY = g_CamMaxYTarget = bottom;
}
