/* MagnetoBoss_StateShoot - MAGNETOBOSS: throw a magnetic globe in direction shotDir - Magneto's
 * air-shot wind-up for that direction (Magneto_AirShotAnim), the globe (Magneto_SpawnShot,
 * in its globe tiles shotTiles, behind or in front as its sprite) - then recover
 * (MagnetoBoss_StateShootRecover) when the animation ends.
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_StateShoot(register MagnetoBoss *self)
{
	register unsigned char dir = self->shotDir;
	register void *anim;

	anim = Magneto_AirShotAnim(dir);
	Obj_ChangeAnim(self, anim);
	Magneto_SpawnShot(self->x, self->y, self->shotDir, 0, 0, self->spr.flags & 4 ? 1 : 0, 1,
		self->shotTiles);
	SET_STATE_WAIT(self, (StateFn)MagnetoBoss_StateShootRecover, 0xFF);
	return self->flags & 8;
}
