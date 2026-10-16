/* Zapper_StateLoopAnim2 - ZAPPER state (kind 2): loop its third animation (Tbl_ZapperAnim3,
 * jt[182]: an animation script like Tbl_ZapperAnim); runs again when it ends (delay $FF).
 *
 * self: the object.
 * Returns flags bit 3. */
#include "xmen2.h"

short Zapper_StateLoopAnim2(register Zapper *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_ZapperAnim3));
	SET_STATE_WAIT(self, Zapper_StateLoopAnim2, 0xFF);
	return self->flags & 8;
}
