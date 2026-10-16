/* Scroll_Init - reset the scroll and camera state: planes 64x32 cells (VDP register $10 = 1), no
 * map drawing or background lock/offset, no scroll requests, the foreground scrolling 1:1 with the
 * camera ($100) and the background not at all, the camera and its limits at 0 (both planes'
 * scroll from it), no streaming position or adjustments, no maps (g_MapTilesEnd 1). Sys_InitHardware
 * calls it last. */
#include "xmen2.h"

void Scroll_Init(void)
{
	register short x;
	register short y;

	VDP_CTRL = (short)0x9001;
	g_MapDrawOff = 0;
	g_BgLockToObj = 0;
	g_BgOffsetOn = 0;
	g_ScrollResync = 0;
	g_BgOffsetX = 0;
	g_BgOffsetY = 0;
	g_FgScrollDirty = 0;
	g_BgScrollDirty = 0;
	g_ColumnVsReq = 0;
	g_VsramReq = 0;
	g_FgScrollFactorX = 0x100;
	g_FgScrollFactorY = 0x100;
	g_BgScrollFactorX = 0;
	g_BgScrollFactorY = 0;
	g_FgScrollBaseX = 0;
	g_FgScrollBaseY = 0;
	g_BgScrollBaseX = 0;
	g_BgScrollBaseY = 0;
	g_CameraX = 0;
	g_CameraY = 0;
	g_FgMapExtraW = 0;
	g_FgMapExtraH = 0;
	g_CamMaxX = 0;
	g_CamMaxXTarget = 0;
	g_CamMinX = 0;
	g_CamMinXTarget = 0;
	g_CamMaxY = 0;
	g_CamMaxYTarget = 0;
	g_CamMinY = 0;
	g_CamMinYTarget = 0;
	g_FgScrollX = x = g_CameraX;
	g_FgScrollY = y = g_CameraY;
	g_BgScrollX = x;
	g_BgScrollY = y;
	g_FgStreamX = 0;
	g_FgStreamY = 0;
	g_BgStreamX = 0;
	g_BgStreamY = 0;
	g_FgScrollAdjX = 0;
	g_FgScrollAdjY = 0;
	g_BgScrollAdjX = 0;
	g_BgScrollAdjY = 0;
	g_FgMapW = 0;
	g_FgMapH = 0;
	g_MapTilesEnd = 1;
	g_BgMapPresent = 0;
	g_BgWrapX = 0;
	g_BgWrapY = 0;
	g_FgMapPresent = 0;
}
