/* HoloBolt_StateActivate - HOLOBOLT state: the flying animation (Tbl_HoloBoltAnimActive), moving
 * from now on (flags6 bit $4000 cleared), sound $CE; HoloBolt_StateEnd when the animation ends
 * (timer $FF).
 *
 * self: the bolt.
 * Returns self->flags & 8. */
#include "xmen2.h"

short HoloBolt_StateActivate(register HoloBolt *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_HoloBoltAnimActive));
	self->flags6 &= ~0x4000;
	Snd_Stop(0xCE);
	Snd_Play(0xCE);
	SET_STATE_WAIT(self, HoloBolt_StateEnd, 0xFF);
	return self->flags & 8;
}
