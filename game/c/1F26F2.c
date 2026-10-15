/* XPlayer_StartFloatFall - start falling slowly (XPlayer_StateFloatFall, now): the scene's fall
 * speed (g_FloatFallSpeed) as the maximum, the character's float-fall animation.
 *
 * self: the player. XPlayer_StateControl starts it in the air when the scene sets a speed.
 * The animation and the state change sit in one do/while (an original macro, it seems): the
 * ROM pops both calls' arguments once, at the end. */
#include "xmen2.h"

void XPlayer_StartFloatFall(XPlayer *self)
{
	self->maxVy = g_FloatFallSpeed;
	do {
		Obj_SetAnim(self, CHAR_PART(self->charRec, floatFallAnim));
		self->state = (StateFn)XPlayer_StateFloatFall;
		XPlayer_StateFloatFall(self);
	} while (0);
}
