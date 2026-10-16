/* VDP_UpdateScroll - the V-blank's scroll upload (VDP_FrameUpdate), by request flags: plane A's x
 * (H-scroll table $F400, negated, 9 bits) and y (VSRAM 0, 8 bits) when g_FgScrollDirty is 1; plane
 * B's ($F402, VSRAM 2) when g_BgScrollDirty is 1 or plane B follows an object (g_BgLockToObj);
 * both y values when g_VsramReq is 1; both x values and 40 column words of VSRAM from the start of
 * g_HScrollBuf when g_ColumnVsReq is 1 (each request flag cleared). The scroll values are the
 * planes' scroll plus their adjustments (g_FgScrollAdjX...). Then each camera limit moves toward its
 * target: by 1 px while the camera sits on the limit, else straight to the target (a lower limit
 * going up, an upper limit going down: no further than the camera). */
#include "xmen2.h"

void VDP_UpdateScroll(void)
{
	register short i;

	if (g_FgScrollDirty == 1) {
		g_FgScrollDirty = 0;
		VDP_CTRL32 = 0x74000003;
		VDP_DATA = -((g_FgScrollX + g_FgScrollAdjX) & 0x1FF);
		VDP_CTRL32 = 0x40000010;
		VDP_DATA = (g_FgScrollY + g_FgScrollAdjY) & 0xFF;
	}
	if (g_BgLockToObj == 1 || g_BgScrollDirty == 1) {
		g_BgScrollDirty = 0;
		VDP_CTRL32 = 0x74020003;
		VDP_DATA = -((g_BgScrollX + g_BgScrollAdjX) & 0x1FF);
		VDP_CTRL32 = 0x40020010;
		VDP_DATA = (g_BgScrollY + g_BgScrollAdjY) & 0xFF;
	}
	if (g_VsramReq == 1) {
		g_VsramReq = 0;
		VDP_CTRL32 = 0x40000010;
		VDP_DATA = (g_FgScrollY + g_FgScrollAdjY) & 0xFF;
		VDP_CTRL32 = 0x40020010;
		VDP_DATA = (g_BgScrollY + g_BgScrollAdjY) & 0xFF;
	}
	if (g_ColumnVsReq == 1) {
		g_ColumnVsReq = 0;
		VDP_CTRL32 = 0x74000003;
		VDP_DATA = -((g_FgScrollX + g_FgScrollAdjX) & 0x1FF);
		VDP_CTRL32 = 0x74020003;
		VDP_DATA = -((g_BgScrollX + g_BgScrollAdjX) & 0x1FF);
		VDP_CTRL32 = 0x40000010;
		for (i = 0; i < 40; i++)
			VDP_DATA = ((s16 *)g_HScrollBuf)[i];
	}
	if (g_CamMinX != g_CamMinXTarget) {
		if (g_CamMinX > g_CamMinXTarget) {
			if (g_CamMinX == g_CameraX)
				g_CamMinX--;
			else
				g_CamMinX = g_CamMinXTarget;
		} else if (g_CamMinX == g_CameraX)
			g_CamMinX++;
		else if (g_CameraX > g_CamMinXTarget)
			g_CamMinX = g_CamMinXTarget;
		else
			g_CamMinX = g_CameraX;
	}
	if (g_CamMaxX != g_CamMaxXTarget) {
		if (g_CamMaxX < g_CamMaxXTarget) {
			if (g_CamMaxX == g_CameraX)
				g_CamMaxX++;
			else
				g_CamMaxX = g_CamMaxXTarget;
		} else if (g_CamMaxX == g_CameraX)
			g_CamMaxX--;
		else if (g_CameraX < g_CamMaxXTarget)
			g_CamMaxX = g_CamMaxXTarget;
		else
			g_CamMaxX = g_CameraX;
	}
	if (g_CamMinY != g_CamMinYTarget) {
		if (g_CamMinY > g_CamMinYTarget) {
			if (g_CamMinY == g_CameraY)
				g_CamMinY--;
			else
				g_CamMinY = g_CamMinYTarget;
		} else if (g_CamMinY == g_CameraY)
			g_CamMinY++;
		else if (g_CameraY > g_CamMinYTarget)
			g_CamMinY = g_CamMinYTarget;
		else
			g_CamMinY = g_CameraY;
	}
	if (g_CamMaxY != g_CamMaxYTarget) {
		if (g_CamMaxY < g_CamMaxYTarget) {
			if (g_CamMaxY == g_CameraY)
				g_CamMaxY++;
			else
				g_CamMaxY = g_CamMaxYTarget;
		} else if (g_CamMaxY == g_CameraY)
			g_CamMaxY--;
		else if (g_CameraY < g_CamMaxYTarget)
			g_CamMaxY = g_CamMaxYTarget;
		else
			g_CamMaxY = g_CameraY;
	}
}
