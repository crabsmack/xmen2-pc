/* BanimazaDoor_StateReset - BANIMAZADOOR state: back down to its home y (py), its active window
 * no longer kept (flags6 $1000 off); next BanimazaDoor_StateWaitPlayer.
 *
 * self: the door. Returns 0.
 * It also tests whether a sprite frame is shown and drops the result. */
#include "xmen2.h"

short BanimazaDoor_StateReset(register BanimazaDoor *self)
{
	self->y = self->py << 8;
	self->flags6 &= ~0x1000;
	(void)!(long)self->spr.frame;
	self->state = BanimazaDoor_StateWaitPlayer;
	return 0;
}
