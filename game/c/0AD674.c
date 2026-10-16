/* SavageA2_CamApply - the Perimeter camera's apply (CAMERA +$0C, SavageA2/A3): scroll to the camera
 * position (plus its offsets and look offset) clamped to the camera limits; plane A by the
 * foreground scroll factors and bases, plane B's x by its factor (Math_MulFix8) and its y so the
 * flood line sits at screen line $100 - (flood - camera y) (the water layer), wrapped when the
 * level says so; then the map streaming hooks run with interrupts off and both planes are marked
 * for the next V-blank.
 *
 * cam: the camera.
 * It loads the old camera position into a register it never uses (kept). */
#include "xmen2.h"

void SavageA2_CamApply(register Camera *cam)
{
	register short x = cam->x + cam->offsetX;
	register short y = cam->y + cam->offsetY + cam->look;
	register short oldX = g_CameraX;
	register short oldY = g_CameraY;

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
	g_FgScrollX = g_FgScrollBaseX + (g_CameraX * g_FgScrollFactorX >> 8);
	g_FgScrollY = g_FgScrollBaseY + (g_CameraY * g_FgScrollFactorY >> 8);
	g_BgScrollX = Math_MulFix8(g_CameraX, g_BgScrollFactorX);
	g_BgScrollY = 0x100 - (g_FloodLineY - g_CameraY);
	if (g_BgWrapX)
		g_BgScrollX &= 0x1FF;
	if (g_BgWrapY)
		g_BgScrollY &= 0xFF;
	do {	/* a loop (a macro's do/while, probably): MPW pops Math_MulFix8's arguments before the branch above */
		Irq_Disable();
		(*g_MapColHook)();
		(*g_MapRowHook)();
		Irq_Enable();
	} while (0);
	g_FgScrollDirty = 1;
	g_BgScrollDirty = g_BgMapPresent;
}
