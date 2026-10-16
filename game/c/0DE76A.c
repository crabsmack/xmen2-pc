/* SentinelDoor_StateOpen1 - SENTINELDOOR opening, first step: frame 1, sound $D6;
 * SentinelDoor_StateOpen2 in 1 frame.
 *
 * self: the door.
 * Returns self->flags & 8. */
#include "xmen2.h"

short SentinelDoor_StateOpen1(register SentinelDoor *self)
{
	(*self->cls->method[20])(self, 1);	/* +$56 SetFrame */
	Snd_Stop(0xD6);
	Snd_Play(0xD6);
	SET_STATE_WAIT(self, SentinelDoor_StateOpen2, 1);
	return self->flags & 8;
}
