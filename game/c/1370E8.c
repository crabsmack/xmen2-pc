/* Hornet_Explode - HORNET Explode (class slot +$7E): the explosion sound ($38, stopped twice
 * first), then a kind-3 hornet bursts into puffs that drop a power-up (ExplodeFx_Burst, $202),
 * any other the parent's (XNPC) Explode.
 *
 * self: the hornet. */
#include "xmen2.h"

void Hornet_Explode(Hornet *self)
{
	Snd_Stop(0x38);
	Snd_Stop(0x38);
	Snd_Play(0x38);
	if (self->kind == 3)
		ExplodeFx_Burst(self, 0x202);
	else
		(*CLASS(Tbl_Hornet_vtable)->parent->method[30])(self);	/* XNPC Explode */
}
