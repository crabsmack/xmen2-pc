/* XPlayer_StateFallOffScreen - falling off the screen (XPlayer_StartFallOffScreen): once the body is
 * more than 224 px below the camera's top, stop, hide, free the camera's y and rejoin the
 * partner in 30 frames (XPlayer_StateRejoin).
 *
 * self: the player. Unhittable meanwhile (hit cooldown held at 60).
 * Returns self->flags & 8 when it switches; nothing while it falls. */
#include "xmen2.h"

short XPlayer_StateFallOffScreen(XPlayer *self)
{
	self->hitCooldown = 60;
	if (self->boxTop - g_CameraY > 0xE0) {
		STOP_MOVING(self);
		(*self->cls->method[20])(self, 0xFF);		/* SetFrame: none */
		g_Camera->camFlags &= ~2;
		SET_STATE_WAIT(self, (StateFn)XPlayer_StateRejoin, 30);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
