/* Tuskette_StatePatrolStart - TUSKETTE walker (kinds 0 and 5), first state: idle animation,
 * then wait for a player to come near (Tuskette_StateWaitNear).
 *
 * self: the object. Never runs in the corpus.
 * Returns self->flags & 8. */
#include "xmen2.h"

short Tuskette_StatePatrolStart(Tuskette *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_TusketteAnim));
	SET_STATE(self, Tuskette_StateWaitNear);
	return self->flags & 8;
}
