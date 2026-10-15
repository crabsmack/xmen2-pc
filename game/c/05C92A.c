/* Tusk_StateChargeStart - TUSK charges the way it faces: no animation, frame 8, speed 900 and
 * acceleration 40 forward, speed limit $640, sound $E6; then Tusk_StateCharging.
 *
 * self: TUSK.
 * Returns 0.
 * The empty if (0) after the state change is a block the original compiled out: its end is a
 * label, and that is where MPW C pops the calls' arguments (between the store and the
 * return value). */
#include "xmen2.h"

short Tusk_StateChargeStart(Tusk *self)
{
	self->anim = 0;
	(*self->cls->method[20])(self, 8);		/* +$56 SetFrame */
	self->vx = self->spr.flags & 0x10 ? -900 : 900;
	self->ax = self->spr.flags & 0x10 ? -40 : 40;
	self->maxVx = 0x640;
	Snd_Stop(0xE6);
	Snd_Play(0xE6);
	self->state = Tusk_StateCharging;
	if (0) {				/* compiled out (see the header) */
	}
	return 0;
}
