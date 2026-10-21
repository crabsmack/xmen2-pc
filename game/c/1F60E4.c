/* Cam_SetPosScaled - Cam_SetPos for a level whose foreground plane scrolls by its own factors.
 *
 * x, y: the camera position wanted. As Cam_SetPos, but the foreground scroll is
 * g_FgScrollBase + camera * g_FgScrollFactor >> 8 on each axis instead of the camera itself, and
 * the background's y by its factor without the foreground base (Level_LoadMaps). */
#include "xmen2.h"

void Cam_SetPosScaled(short x, short y)
{
	short oldX = g_CameraX;
	short oldY = g_CameraY;

	g_CameraX = x;
	g_CameraY = y;
	if (g_CameraX > CAM_HI(g_CamMinX, g_CamMaxX))
		g_CameraX = CAM_HI(g_CamMinX, g_CamMaxX);
	if (g_CameraX < CAM_LO(g_CamMinX, g_CamMaxX))
		g_CameraX = CAM_LO(g_CamMinX, g_CamMaxX);
	if (g_CameraY > g_CamMaxY)
		g_CameraY = g_CamMaxY;
	if (g_CameraY < g_CamMinY)
		g_CameraY = g_CamMinY;
	if (!g_BgOffsetOn && !g_BgLockToObj && oldX == g_CameraX && oldY == g_CameraY
	    && !g_ScrollResync)
		return;
	g_FgScrollX = g_FgScrollBaseX + (g_CameraX * g_FgScrollFactorX >> 8);
	g_FgScrollY = g_FgScrollBaseY + (g_CameraY * g_FgScrollFactorY >> 8);
	if (g_ScrollResync)
		g_FgStreamY += 0x100;
	g_ScrollResync = 0;
	if (g_BgLockToObj) {
		g_BgScrollX = g_BgOffsetX - ((((Actor *)g_BgLockObj)->x >> 8) - g_CameraX);
		g_BgScrollY = g_BgOffsetY - ((((Actor *)g_BgLockObj)->y >> 8) - g_CameraY);
	} else {
		g_BgScrollX = Math_MulFix8(g_CameraX, g_BgScrollFactorX);
		g_BgScrollY = g_BgScrollBaseY + (g_CameraY * g_BgScrollFactorY >> 8);
		if (g_BgOffsetOn) {
			g_BgScrollX += g_BgOffsetX;
			g_BgScrollY += g_BgOffsetY;
		}
	}
	if (g_BgWrapX)
		g_BgScrollX &= 0x1FF;
	if (g_BgWrapY)
		g_BgScrollY &= 0xFF;
	Irq_Disable();
	(*g_MapColHook)();
	(*g_MapRowHook)();
	Irq_Enable();
	g_FgScrollDirty = 1;
	g_BgScrollDirty = g_BgMapPresent;
}
