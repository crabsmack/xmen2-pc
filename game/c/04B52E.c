/* Apocalypse_StateEntrance - APOCALYPSE script action (Apocalypse_ScriptA/B's first step): enter
 * from above the top-left corner and glide to (256, 96).
 *
 * self: the boss.
 * Starts at (-10, -20) px facing left with frame $82, restarts sound $79 and starts the damped
 * move (shift 5); Apocalypse_StateWaitEntrance waits for it. Returns self->flags & 8. */
#include "xmen2.h"

short Apocalypse_StateEntrance(register Apocalypse *self)
{
	self->x = 0xF600;
	self->y = -0x1400;
	self->spr.flags |= 0x10;
	(*self->cls->method[20])(self, 0x82);		/* +$56 SetFrame */
	Snd_Stop(0x79);
	Snd_Play(0x79);
	Apocalypse_MoveTo(self, 0x10000, 0x6000, 5);
	SET_STATE(self, (StateFn)Apocalypse_StateWaitEntrance);
	return self->flags & 8;
}
