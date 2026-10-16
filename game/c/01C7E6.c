/* YoyoAcolyte_ActThrowHigh - YOYOACOLYTE script action: the wind-up animation
 * Tbl_YoyoAcolyteAnim2, then throw the yoyo in a high arc (YoyoAcolyte_StateThrowHigh when it
 * ends: delay $FF).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short YoyoAcolyte_ActThrowHigh(register YoyoAcolyte *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_YoyoAcolyteAnim2));
	SET_STATE_WAIT(self, YoyoAcolyte_StateThrowHigh, 0xFF);
	return self->flags & 8;
}
