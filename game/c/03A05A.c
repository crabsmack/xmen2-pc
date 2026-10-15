/* BanimazaDoor_StateWaitPlayer - BANIMAZADOOR state: wait until the nearest player is less than
 * $82 px away in x; then keep the door active (flags6 $1000) and go to
 * BanimazaDoor_StateStartRise.
 *
 * self: the door. Returns 0 when it goes on, no value while waiting. */
#include "xmen2.h"

short BanimazaDoor_StateWaitPlayer(register BanimazaDoor *self)
{
	register long dx;

	dx = self->x - Player_Nearest((Actor *)self)->x;
	if (ABS(dx) < 0x8200) {
		self->flags6 |= 0x1000;
		self->state = BanimazaDoor_StateStartRise;
		return 0;
	}
	NO_RETURN_VALUE
}
