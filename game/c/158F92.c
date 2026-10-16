/* FluxBomb_Explode - FLUXBOMB state after the flash: no animation, the bomb sprite hidden, sound
 * $7C, the burst (FluxBomb_StartBurst: the blast hits), then FluxBomb_StateBurst.
 *
 * self: the bomb.  Returns self->flags & 8. */
#include "xmen2.h"

short FluxBomb_Explode(register FluxBomb *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 0xFF);			/* +$56 SetFrame: none */
	Snd_Stop(0x7C);
	Snd_Play(0x7C);
	FluxBomb_StartBurst(self);
	SET_STATE(self, (StateFn)FluxBomb_StateBurst);
	return self->flags & 8;
}
