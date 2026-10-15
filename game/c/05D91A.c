/* Tuskette_StateRunRightEnd - TUSKETTE runner at its right turning point: idle animation,
 * flags6 bit $4000 on, then after 30 frames run left (Tuskette_StateRunLeft).
 *
 * self: the object.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateRunRightEnd(Tuskette *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim));
	self->flags6 |= 0x4000;
	SET_STATE_WAIT(self, Tuskette_StateRunLeft, 30);
	return self->flags & 8;
}
