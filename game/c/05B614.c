/* Boulder_StateStartRoll - first state of a falling BOULDER (the boulder rain): sound $97, the
 * rolling animation (Tbl_BoulderAnim4), then bouncing (Boulder_StateBounce).
 *
 * self: the boulder. Returns self->flags & 8. */
#include "xmen2.h"

short Boulder_StateStartRoll(register Boulder *self)
{
	Snd_Stop(0x97);
	Snd_Play(0x97);
	Obj_SetAnim(self, CODEPTR(Tbl_BoulderAnim4));
	SET_STATE(self, Boulder_StateBounce);
	return self->flags & 8;
}
