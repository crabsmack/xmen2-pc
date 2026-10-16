/* AvalonB1_ScrollCameraTo - put the AvalonB1 intro camera at (x, y), clamped to the level's
 * camera limits, and redraw the map there at once (the column and row hooks, interrupts off);
 * the scroll is uploaded at the next V-blank. A pending g_ScrollResync moves the row streaming
 * on by $100 first.
 *
 * x, y: camera position in pixels. */
#include "xmen2.h"

void AvalonB1_ScrollCameraTo(short x, short y)
{
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
	g_FgScrollX = g_CameraX;
	g_FgScrollY = g_CameraY;
	if (g_ScrollResync)
		g_FgStreamY += 0x100;
	g_ScrollResync = 0;
	Irq_Disable();
	(*g_MapColHook)();
	(*g_MapRowHook)();
	Irq_Enable();
	g_FgScrollDirty = 1;
	g_BgScrollDirty = g_BgMapPresent;
}
