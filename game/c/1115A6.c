/* Deathbird_LaunchBurst - DEATHBIRD class slot +$6E (Deathbird_StateThrowBurst, _StateDiveLow):
 * set off the 8 burst projectiles from 20 px left of and 50 px above it, fanned out ahead of it
 * (each its own speed and acceleration, mirrored in x when it faces left) and down.
 *
 * self: the boss. Nothing without a burst record; the burst timer (Deathbird_UpdateBurst) moves
 * them. */
#include "xmen2.h"

void Deathbird_LaunchBurst(Deathbird *self)
{
	register short i;
	register DeathbirdBurst *b;
	register BurstShot *s;

	b = self->burst;
	if (b) {
		for (i = 0; i < 8; i++) {
			s = &b->shot[i];
			s->x = self->x + -0x1400;
			s->y = self->y + -0x3200;
		}
		if (!(self->spr.flags & 0x10)) {
			s = &b->shot[0];
			s->vx = 0x3D4;
			s->ax = 6;
			s = &b->shot[1];
			s->vx = 0x456;
			s->ax = 3;
			s = &b->shot[2];
			s->vx = 0x320;
			s->ax = 1;
			s = &b->shot[3];
			s->vx = 0x4B0;
			s->ax = 2;
			s = &b->shot[4];
			s->vx = 0x2BC;
			s->ax = 1;
			s = &b->shot[5];
			s->vx = 0x2BC;
			s->ax = 0x16;
			s = &b->shot[6];
			s->vx = 0x258;
			s->ax = 0x10;
			s = &b->shot[7];
			s->vx = 0x30C;
			s->ax = 8;
		} else {
			s = &b->shot[0];
			s->vx = -0x3D4;
			s->ax = -6;
			s = &b->shot[1];
			s->vx = -0x456;
			s->ax = -3;
			s = &b->shot[2];
			s->vx = -0x320;
			s->ax = -1;
			s = &b->shot[3];
			s->vx = -0x4B0;
			s->ax = -2;
			s = &b->shot[4];
			s->vx = -0x2BC;
			s->ax = -1;
			s = &b->shot[5];
			s->vx = -0x2BC;
			s->ax = -0x16;
			s = &b->shot[6];
			s->vx = -0x258;
			s->ax = -0x10;
			s = &b->shot[7];
			s->vx = -0x30C;
			s->ax = -8;
		}
		s = &b->shot[0];
		s->vy = 0x3D4;
		s->ay = 6;
		s = &b->shot[1];
		s->vy = 0x334;
		s->ay = 1;
		s = &b->shot[2];
		s->vy = 0x44C;
		s->ay = 2;
		s = &b->shot[3];
		s->vy = 0x2BC;
		s->ay = 1;
		s = &b->shot[4];
		s->vy = 0x4B0;
		s->ay = 2;
		s = &b->shot[5];
		s->vy = 0x258;
		s->ay = 0x10;
		s = &b->shot[6];
		s->vy = 0x2BC;
		s->ay = 0x16;
		s = &b->shot[7];
		s->vy = 0x30C;
		s->ay = 8;
	}
}
