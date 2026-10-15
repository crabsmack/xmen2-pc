/* Cam_DefaultUpdate - the default camera update: run the camera's steps.
 *
 * self: the camera. Keeps the position in prevX/prevY, then runs each step function that is
 * set: track, ease, shake, lookFn, apply. g_CamActive = 1 while a player is alive or the
 * background follows an object (g_BgLockToObj), else 0. */
#include "xmen2.h"

void Cam_DefaultUpdate(Camera *self)
{
	register Camera *c = self;
	register CamFn f;

	c->prevY = c->y;
	c->prevX = c->x;
	if (f = c->track)
		f(c);
	if (f = c->ease)
		f(c);
	if (f = c->shake)
		f(c);
	if (f = c->lookFn)
		f(c);
	if (f = c->apply)
		f(c);
	g_CamActive = IS_ALIVE(&g_Player1) || IS_ALIVE(&g_Player2) || g_BgLockToObj;
}
