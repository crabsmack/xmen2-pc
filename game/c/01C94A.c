/* YoyoAcolyte_StateDie - YOYOACOLYTE hurt state (death): stop sound $DB, an explosion at the
 * centre of its body box, the yoyo gone (yoyoFlags 0), then the medium burst death
 * (ExplodeFx_BurstMedium).
 *
 * self: the object. */
#include "xmen2.h"

void YoyoAcolyte_StateDie(register YoyoAcolyte *self)
{
	register long cx = (self->boxRight + self->boxLeft) << 7;
	register long cy = (self->boxTop + self->boxBottom) << 7;

	Snd_Stop(0xDB);
	Explosion_Spawn(cx, cy, 0);
	self->yoyoFlags = 0;
	ExplodeFx_BurstMedium(self);
}
