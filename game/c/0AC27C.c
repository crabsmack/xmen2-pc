/* FloatBubbleExplode_StateStart - FLOATBUBBLEEXPLODE first state: play the burst animation
 * (Tbl_FloatBubbleExplodeAnim); FloatBubbleExplode_StateEnd at its end.
 *
 * self: the burst.
 * Returns self->flags & 8, as every state. */
#include "xmen2.h"

short FloatBubbleExplode_StateStart(FloatBubbleExplode *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_FloatBubbleExplodeAnim));
	SET_STATE_WAIT(self, (StateFn)FloatBubbleExplode_StateEnd, 0xFF);
	return self->flags & 8;
}
