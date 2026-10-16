/* FluxBomb_StartHeld - FLUXBOMB state, first of the bomb Magneto holds: at most 45 frames
 * (countdown), sound $7A, the flight animation (Tbl_FluxBombAnim21), then FluxBomb_StateHeld.
 *
 * self: the bomb.  Returns self->flags & 8. */
#include "xmen2.h"

short FluxBomb_StartHeld(register FluxBomb *self)
{
	self->countdown = 0x2D;
	Snd_Stop(0x7A);
	Snd_Play(0x7A);
	Obj_SetAnim(self, CODEPTR(Tbl_FluxBombAnim21));
	SET_STATE(self, FluxBomb_StateHeld);
	return self->flags & 8;
}
