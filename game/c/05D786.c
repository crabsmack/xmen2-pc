/* Tuskette_StateStop - TUSKETTE of a pair, far enough out: flags6 bit $4000 on, idle animation,
 * and when it has played (wait $FF) rest like a runner (Tuskette_StateRestIdle).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateStop(Tuskette *self)
{
	self->flags6 |= 0x4000;
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim));
	SET_STATE_WAIT(self, Tuskette_StateRestIdle, 0xFF);
	return self->flags & 8;
}
