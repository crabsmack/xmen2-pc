/* LaserCannon_StateFire - LASERCANNON state: fire one shot of the burst, frame 3: a gauss bullet
 * (GaussBullet_Fire, no owner) 30 px ahead and 27 px up (down when v-flipped) flying diagonally
 * by that offset, and its sound ($F3, restarted). After the last shot, LaserCannon_StateRetract
 * in 15 frames; else LaserCannon_StateAim after the shot delay (-1: at once: it runs it now and
 * returns its result).
 *
 * self: the cannon. Returns flags & 8.
 * Bullet flags: $8000 sprite priority, $800 facing left (the offset mirrored), $A always. */
#include "xmen2.h"

short LaserCannon_StateFire(LaserCannon *self)
{
	(*self->cls->method[20])(self, 3);		/* +$56 SetFrame */
	{
		long bx = self->x;
		register long by = self->y;
		register short dx = 30;
		register short dy = -27;
		register long flags = self->spr.flags & 4 ? 0x8000 : 0;

		if (self->spr.flags & 8)
			dy = -dy;
		if (self->spr.flags & 0x10) {
			dx = -dx;
			flags |= 0x800;
		}
		bx += (long)dx << 8;
		by += (long)dy << 8;
		flags |= 0xA;
		GaussBullet_Fire(bx, by, 0, dx, dy, flags);
	}
	Snd_Stop(0xF3);
	Snd_Play(0xF3);
	if (--self->shotsLeft) {
		if (self->shotDelay == -1) {
			self->state = (StateFn)LaserCannon_StateAim;
			return LaserCannon_StateAim(self);
		}
		SET_STATE_WAIT(self, (StateFn)LaserCannon_StateAim, self->shotDelay);
		return self->flags & 8;
	}
	SET_STATE_WAIT(self, (StateFn)LaserCannon_StateRetract, 15);
	return self->flags & 8;
}