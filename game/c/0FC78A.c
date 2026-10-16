/* SiberiaB1_CamApply - the Blizzard camera's apply function (CAMERA +$0C, SiberiaB1_InitCamera):
 * the scroll from the camera. g_CameraX/Y = position + transition offset (+ look offset in y),
 * clamped to the camera limits; plane A scroll moves by the wind (g_BgOffsetX/Y, set by
 * SiberiaB1_StepWind) and by plane B's own change; plane B scroll follows the camera through the
 * parallax factors. Both wrap (512 x 256), then the map streaming hooks run with interrupts off
 * and both planes are marked for the next V-blank.
 *
 * cam: the camera. */
#include "xmen2.h"

void SiberiaB1_CamApply(register Camera *cam)
{
	register short x;
	register short y;
	register short oldX;
	register short oldY;

	x = cam->x + cam->offsetX;
	y = (cam->y + cam->offsetY) + cam->look;
	oldX = g_BgScrollX;
	oldY = g_BgScrollY;
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
	g_FgScrollX += g_BgOffsetX;
	g_FgScrollY += g_BgOffsetY;
	g_BgScrollX = g_BgScrollBaseX + (g_CameraX * g_BgScrollFactorX >> 8);
	g_BgScrollY = g_BgScrollBaseY + (g_CameraY * g_BgScrollFactorY >> 8);
	g_FgScrollX += g_BgScrollX - oldX;
	g_FgScrollY += g_BgScrollY - oldY;
	if (g_ScrollResync)
		g_BgStreamY += 0x100;
	g_ScrollResync = 0;
	g_FgScrollX &= 0x1FF;
	g_FgScrollY &= 0xFF;
	Irq_Disable();
	(*g_MapColHook)();
	(*g_MapRowHook)();
	Irq_Enable();
	g_BgScrollDirty = 1;
	g_FgScrollDirty = g_BgMapPresent;
}
