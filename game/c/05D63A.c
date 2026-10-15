/* Tuskette_StateChaseStart - TUSKETTE of a pair (kinds 6/7), a player on its floor: no vertical
 * motion, flags6 bit $4000 off, walk animation, then run at its partner
 * (Tuskette_StateChaseTarget). The x speed and acceleration Tuskette_Init gave it carry it.
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StateChaseStart(Tuskette *self)
{
	self->ay = 0;
	self->vy = 0;
	self->flags6 &= ~0x4000;
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim2));
	SET_STATE(self, Tuskette_StateChaseTarget);
	return self->flags & 8;
}
