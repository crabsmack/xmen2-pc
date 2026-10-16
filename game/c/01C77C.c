/* YoyoAcolyte_ActThrowLevel - YOYOACOLYTE script action: the wind-up animation
 * Tbl_YoyoAcolyteAnim2, then throw the yoyo level (YoyoAcolyte_StateThrowLevel when it ends:
 * delay $FF).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short YoyoAcolyte_ActThrowLevel(register YoyoAcolyte *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_YoyoAcolyteAnim2));
	SET_STATE_WAIT(self, YoyoAcolyte_StateThrowLevel, 0xFF);
	return self->flags & 8;
}
