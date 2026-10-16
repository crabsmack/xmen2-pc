/* Zapper_StateLoopAnim0 - ZAPPER state (kind 0): loop the animation Tbl_ZapperAnim; runs again
 * when it ends (delay $FF).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short Zapper_StateLoopAnim0(register Zapper *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_ZapperAnim));
	SET_STATE_WAIT(self, Zapper_StateLoopAnim0, 0xFF);
	return self->flags & 8;
}
