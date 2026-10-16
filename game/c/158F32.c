/* FluxBomb_Detonate - FLUXBOMB state: the detonation flash (Tbl_FluxBombAnim23), then
 * FluxBomb_Explode when the animation ends (timer $FF).
 *
 * self: the bomb.  Returns self->flags & 8. */
#include "xmen2.h"

short FluxBomb_Detonate(register FluxBomb *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_FluxBombAnim23));
	SET_STATE_WAIT(self, FluxBomb_Explode, 0xFF);
	return self->flags & 8;
}
