/* Cam_SetPos - scroll to a camera position, clamped to the camera limits.
 *
 * x, y: the camera position wanted (world pixels of the screen's top left).
 * g_CameraX/Y = x/y clamped to g_CamMinX..g_CamMaxX and g_CamMinY..g_CamMaxY. When it changed
 * (or a background offset or lock is on, or g_ScrollResync asks), the foreground scroll follows
 * the camera, the background either follows the object g_BgLockObj (g_BgLockToObj) or the
 * parallax factors (+ g_BgOffset when on), wrapped when the level says so; then the map streaming
 * hooks run with interrupts off and both planes are marked for the next V-blank. */
#include "xmen2.h"

void Cam_SetPos(short x, short y)
{
	short oldX = g_CameraX;
	short oldY = g_CameraY;

	g_CameraX = x;
	g_CameraY = y;
	if (g_CameraX > g_CamMaxX)
		g_CameraX = g_CamMaxX;
	if (g_CameraX < g_CamMinX)
		g_CameraX = g_CamMinX;
	if (g_CameraY > g_CamMaxY)
		g_CameraY = g_CamMaxY;
	if (g_CameraY < g_CamMinY)
		g_CameraY = g_CamMinY;
	if (!g_BgOffsetOn && !g_BgLockToObj && oldX == g_CameraX && oldY == g_CameraY
	    && !g_ScrollResync)
		return;
	g_FgScrollX = g_CameraX;
	g_FgScrollY = g_CameraY;
	if (g_ScrollResync)
		g_FgStreamY += 0x100;
	g_ScrollResync = 0;
	if (g_BgLockToObj) {
		g_BgScrollX = g_BgOffsetX - ((((Actor *)g_BgLockObj)->x >> 8) - g_CameraX);
		g_BgScrollY = g_BgOffsetY - ((((Actor *)g_BgLockObj)->y >> 8) - g_CameraY);
	} else {
		g_BgScrollX = Math_MulFix8(g_CameraX, g_BgScrollFactorX);
		g_BgScrollY = g_BgScrollBaseY + ((g_CameraY - g_FgScrollBaseY) * (long)g_BgScrollFactorY >> 8);
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
