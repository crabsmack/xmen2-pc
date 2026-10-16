/* Zapper_StateLoopAnim1 - ZAPPER state (kind 1): loop the animation Tbl_ZapperAnim2; runs again
 * when it ends (delay $FF).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short Zapper_StateLoopAnim1(register Zapper *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_ZapperAnim2));
	SET_STATE_WAIT(self, Zapper_StateLoopAnim1, 0xFF);
	return self->flags & 8;
}
