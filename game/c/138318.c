/* LaserCannon_StatePickTarget - LASERCANNON state: the nearest live player becomes its target; with
 * one it watches it for 60 frames (countdown; LaserCannon_StateWatchTarget from the next update),
 * with none it stays the state.
 *
 * self: the cannon. Returns flags & 8 with a target; with none no value (d0 is the null target). */
#include "xmen2.h"

short LaserCannon_StatePickTarget(register LaserCannon *self)
{
	register Actor *t;

	if ((t = self->target = Player_NearestLive(self))) {
		self->countdown = 60;
		SET_STATE(self, (StateFn)LaserCannon_StateWatchTarget);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
