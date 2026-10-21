/* SpaceA1_CamApply - the Space Elevator camera's apply (CAMERA +$0C, SpaceA1_InitCamera): the
 * camera position, clamped to the level's camera limits, becomes the plane A scroll and plane B's
 * x; then the map streaming hooks run with interrupts off and the planes are marked for the next
 * V-blank (plane B only when the level has one).
 *
 * cam: the camera.
 * It loads the old camera position into a register it never uses (kept: MPW only keeps those
 * loads when the variable is read somewhere, here in compiled-out code). */
#include "xmen2.h"

void SpaceA1_CamApply(register Camera *cam)
{
	short old;
	register short x;
	register short y;

	x = cam->x;
	y = cam->y;
	old = g_CameraX;
	old = g_CameraY;
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
	g_FgScrollX = g_CameraX;
	g_FgScrollY = g_CameraY;
	g_BgScrollX = g_FgScrollX;
	Irq_Disable();
	(*g_MapColHook)();
	(*g_MapRowHook)();
	Irq_Enable();
	g_FgScrollDirty = 1;
	g_BgScrollDirty = g_BgMapPresent;
	if (0)
		g_CameraX = old;
}
