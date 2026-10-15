/* Cam_EaseOffset - camera step (ease): move the transition offset back toward 0.
 *
 * self: the camera. After a follow-mode switch (g_CamModeSwitched, cleared here) the offset
 * first takes the jump of the position (previous - new), so the screen doesn't jump. Each axis
 * then moves easeStep toward 0 (0 when within it); y twice with two players alive. */
#include "xmen2.h"

/* v moved s toward 0 (0 when within s) */
#define EASE_TO_0(v, s) \
	if ((v) > (s)) \
		(v) -= (s); \
	else if ((v) < -(s)) \
		(v) += (s); \
	else \
		(v) = 0

void Cam_EaseOffset(Camera *self)
{
	register Camera *c = self;
	short step = c->easeStep;
	short ox = c->offsetX;
	short oy = c->offsetY;

	if (g_CamModeSwitched) {
		g_CamModeSwitched = 0;
		ox += c->prevX - c->x;
		oy += c->prevY - c->y;
	}
	EASE_TO_0(ox, step);
	c->offsetX = ox;
	EASE_TO_0(oy, step);
	if (g_PlayersAlive == 2) {
		EASE_TO_0(oy, step);
	}
	c->offsetY = oy;
}
