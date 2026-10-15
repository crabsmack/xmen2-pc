/* Cam_PanTo - CAMERA slot +$36: pan the camera to a point.
 *
 * self: the camera; x, y: the target camera position; speedX, speedY: pixels per frame (their
 * size is used); lockX, lockY: keep that axis locked afterwards (camFlags 1 / 2, until
 * Cam_UnlockAxes).
 * The pan starts from the current scroll (g_CameraX/Y), without offset easing or look step;
 * the tracking function is saved in savedTrack and Cam_PanStep takes over. */
#include "xmen2.h"

void Cam_PanTo(Camera *self, short x, short y, register short speedX, register short speedY,
	char lockX, char lockY)
{
	register Camera *c = self;

	c->speedX = ABS(speedX);
	c->speedY = ABS(speedY);
	c->panX = x;
	c->panY = y;
	c->x = g_CameraX;
	c->y = g_CameraY;
	c->ease = 0;
	c->lookFn = 0;
	if (lockX)
		c->camFlags |= 1;
	if (lockY)
		c->camFlags |= 2;
	c->savedTrack = c->track;
	c->track = Cam_PanStep;
}
