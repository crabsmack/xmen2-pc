/* BigBio_StateFormRing - BIGBIO in the air: stop, and move the orbs onto the ring (as
 * BigBio_UpdateOrbRing places them, without spinning it or hurting anyone), up to 8 px per frame
 * in x and y. When all are on it: countdown 15, sound $FB, spin up (BigBio_StateSpinUp).
 *
 * self: the boss. Returns flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short BigBio_StateFormRing(register BigBio *self)
{
	short s, c;
	short i;
	short px, py, step, rs, r, tx, ty;
	register BigBioOrb *orb;
	char done;
	short angle, dx, dy;

	orb = g_BigBioOrbs;
	done = 1;
	angle = 0;
	px = self->x >> 8;
	py = self->y >> 8;
	STOP_MOVING(self);
	step = 24;
	r = 48 - (self->ringSpeed >> 4);
	rs = r >> 2;
	for (i = 0; i < 16; i++) {
		if (i & 1) {
			s = Math_SinDeg(angle + self->ringAngle) * rs >> 16;
			c = Math_CosDeg(angle + self->ringAngle) * rs >> 16;
			tx = px + (s * Math_SinDeg(orb->spin << 1) >> 16);
		} else {
			s = Math_SinDeg(angle + self->ringAngle) * r >> 16;
			c = Math_CosDeg(angle + self->ringAngle) * r >> 16;
			tx = px + (s * Math_SinDeg(orb->spin) >> 16);
		}
		orb->spin = self->ringSpin;
		dx = tx - orb->x;
		if (dx > 8) {
			orb->x += 8;
			done = 0;
		} else if (dx > 0) {
			orb->x += dx;
			done = 0;
		} else if (dx < -8) {
			orb->x -= 8;
			done = 0;
		} else if (dx < 0) {
			orb->x += dx;
			done = 0;
		}
		ty = py - c;
		dy = ty - orb->y;
		if (dy > 8) {
			orb->y += 8;
			done = 0;
		} else if (dy > 0) {
			orb->y += dy;
			done = 0;
		} else if (dy < -8) {
			orb->y -= 8;
			done = 0;
		} else if (dy < 0) {
			orb->y += dy;
			done = 0;
		}
		angle += step;
		orb++;
	}
	if (done) {
		self->countdown = 15;
		Snd_Stop(0xFB);
		Snd_Play(0xFB);
		SET_STATE(self, BigBio_StateSpinUp);
		return self->flags & 8;
	}
	SET_STATE(self, BigBio_StateFormRing);
	return self->flags & 8;
}
