/* BioSentinel_FireBullet - fire a BIOBULLET from the sentinel's gun and count the shot: up
 * (dir -1: from $34/-$4E px (8 fraction bits $3400/-$4E00) away, velocity 600/-180), level (0:
 * $3A00/-$3300, $3C0/0) or down (1, or anything else: $3000/-$2B00, 600/180), mirrored in x when
 * facing left; the bullet takes the sentinel's priority. No bullet (none free): no shot counted.
 *
 * self: the sentinel.  dir: -1 up, 0 level, 1 down. */
#include "xmen2.h"

void BioSentinel_FireBullet(register BioSentinel *self, char dir)
{
	register short vx;
	register long dx;
	register short vy;
	register long dy;
	register Actor *b;

	switch (dir) {
	case -1:
		dx = 0x3400;
		dy = -0x4E00;
		vx = 600;
		vy = -180;
		break;
	case 0:
		dx = 0x3A00;
		dy = -0x3300;
		vx = 0x3C0;
		vy = 0;
		break;
	case 1:
	default:
		dx = 0x3000;
		dy = -0x2B00;
		vx = 600;
		vy = 180;
		break;
	}
	if (self->spr.flags & 0x10) {
		dx = -dx;
		vx = -vx;
	}
	if ((b = (Actor *)(*CLASS(Tbl_BioBullet_vtable)->method[2])(0, CLASS(Tbl_BioBullet_vtable),	/* New */
		self->x + dx, self->y + dy, 0)) != 0) {
		BioBullet_SetVelocity(b, vx, vy, self->spr.flags & 4);
		--self->shotsLeft;
	}
}

