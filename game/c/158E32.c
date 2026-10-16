/* FluxBomb_StartThrown - FLUXBOMB state, first of a bomb the clone throws: sound $7A, the flight
 * animation (Tbl_FluxBombAnim21), then FluxBomb_StateThrown.
 *
 * self: the bomb.  Returns self->flags & 8. */
#include "xmen2.h"

short FluxBomb_StartThrown(register FluxBomb *self)
{
	Snd_Stop(0x7A);
	Snd_Play(0x7A);
	Obj_SetAnim(self, CODEPTR(Tbl_FluxBombAnim21));
	SET_STATE(self, FluxBomb_StateThrown);
	return self->flags & 8;
}
