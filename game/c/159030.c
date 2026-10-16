/* FluxBomb_StateHeld - FLUXBOMB state of the bomb Magneto holds: it flies on while he still holds
 * it (held) and its 45 frames last; then it stops (frozen, flags6 $4000), plays the implode
 * animation (Tbl_FluxBombAnim22) and implodes (FluxBomb_StartImplode, then FluxBomb_StateImplode).
 *
 * self: the bomb.  Returns self->flags & 8. */
#include "xmen2.h"

short FluxBomb_StateHeld(register FluxBomb *self)
{
	if (self->held && self->countdown > 0) {
		SET_STATE(self, FluxBomb_StateHeld);
		return self->flags & 8;
	}
	self->flags6 |= 0x4000;
	Obj_SetAnim(self, CODEPTR(Tbl_FluxBombAnim22));
	FluxBomb_StartImplode(self);
	SET_STATE(self, FluxBomb_StateImplode);
	return self->flags & 8;
}
