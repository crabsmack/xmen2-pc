/* BigBio_ActSpit3 - BIGBIO script action (the script's first): spit animation, a volley of 3
 * shots once it has played (BigBio_StateSpitStart).
 *
 * self: the boss. Returns flags & 8. */
#include "xmen2.h"

short BigBio_ActSpit3(register BigBio *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_Anim_1206F4));
	self->shotsLeft = 3;
	SET_STATE_WAIT(self, BigBio_StateSpitStart, 0xFF);
	return self->flags & 8;
}
