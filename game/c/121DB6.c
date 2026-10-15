/* BigBio_UpdateOrbRing - BIGBIO's orb ring, once per frame of the flying states: hurt the players
 * inside it, put the 16 orbs on the ring and spin it.
 *
 * self: the boss. The ring radius r is 48 px - ringSpeed/16 (it tightens as it spins up); odd
 *       orbs are on a ring of r/4. A live player not blinking whose second box overlaps the
 *       square of +-r px around the body takes damage 1 (TakeDamage type 4, from self).
 *       Orb k sits at angle a = ringAngle + 24k: x = body + r sin(a) sin(w), y = body - r cos(a),
 *       w being the orb's spin (last frame's ringSpin; doubled for the odd orbs).
 * Afterwards the ring spins: ringSpin and ringAngle += ringSpeed/16 (wrapped at 360), and
 * ringSpeed grows by 10 until r is 20 px or less.
 * Returns 1 once the ring is at full speed (r <= 20), else 0. Never runs in the corpus. */
#include "xmen2.h"

char BigBio_UpdateOrbRing(register BigBio *self)
{
	short s, c;
	BigBioOrb *orb;
	char full;
	short py, step, rs;
	register XPlayer *p;
	short px, angle, r;
	register long n;
	register short i;

	orb = g_BigBioOrbs;
	angle = 0;
	full = 1;
	px = self->x >> 8;
	py = self->y >> 8;
	step = 24;
	rs = (r = 48 - (self->ringSpeed >> 4)) >> 2;
	for (n = 0; n < 2; ++n) {
		if (IS_ALIVE(n == 0 ? (p = &g_Player1) : (p = &g_Player2)) && !p->hitCooldown
		    && p->box2Left <= px + r && p->box2Right >= px - r
		    && p->box2Top <= py + r && p->box2Bottom >= py - r)
			(*p->cls->method[8])(p, self, 4, 1, -1, -1);		/* TakeDamage */
	}
	for (i = 0; i < 16; orb++, i++) {
		if (i & 1) {
			s = Math_SinDeg(angle + self->ringAngle) * rs >> 16;
			c = Math_CosDeg(angle + self->ringAngle) * rs >> 16;
			orb->x = (s * Math_SinDeg(orb->spin << 1) >> 16) + px;
		} else {
			s = Math_SinDeg(angle + self->ringAngle) * r >> 16;
			c = Math_CosDeg(angle + self->ringAngle) * r >> 16;
			orb->x = (s * Math_SinDeg(orb->spin) >> 16) + px;
		}
		orb->spin = self->ringSpin;
		orb->y = py - c;
		angle += step;
	}
	if (r <= 20)
		full = 1;
	else {
		full = 0;
		self->ringSpeed += 10;
	}
	self->ringSpin += self->ringSpeed >> 4;
	self->ringAngle += self->ringSpeed >> 4;
	if (self->ringSpin > 360)
		self->ringSpin -= 360;
	if (self->ringAngle > 360)
		self->ringAngle -= 360;
	return full;
}
