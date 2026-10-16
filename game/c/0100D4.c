/* BigAcBullet_StateLaunch - BIGACBULLET first state: shot sound $EE (restarted), face and fly the
 * way of its direction at $1000, the flying animation (Tbl_BigAcBulletAnim), then
 * BigAcBullet_StateFly from the next update.
 *
 * self: the bullet.
 * Direction 0 flies right; any other flies left (h-flipped when over 90 degrees).
 * Returns flags bit 3. */
#include "xmen2.h"

short BigAcBullet_StateLaunch(register BigAcBullet *self)
{
	Snd_Stop(0xEE);
	Snd_Play(0xEE);
	if (self->dir > 0x5A)
		self->spr.flags |= 0x10;
	switch (self->dir) {
	case 0:
		self->vx = 0x1000;
		self->vy = 0;
		break;
	case 0xB4:
	default:
		self->vx = -0x1000;
		self->vy = 0;
		break;
	}
	Obj_SetAnim(self, CODEPTR(Tbl_BigAcBulletAnim));
	SET_STATE(self, (StateFn)BigAcBullet_StateFly);
	return self->flags & 8;
}
