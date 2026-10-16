/* YoyoAcolyte_ActThrowLow - YOYOACOLYTE script action: the wind-up animation
 * Tbl_YoyoAcolyteAnim2, then throw the yoyo in a low arc (YoyoAcolyte_StateThrowLow when it
 * ends: delay $FF).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short YoyoAcolyte_ActThrowLow(register YoyoAcolyte *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_YoyoAcolyteAnim2));
	SET_STATE_WAIT(self, YoyoAcolyte_StateThrowLow, 0xFF);
	return self->flags & 8;
}
