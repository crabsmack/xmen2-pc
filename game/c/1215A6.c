/* BigBio_SpitSpawn - launch a spit projectile from BIGBIO's mouth, diagonally down and forward.
 *
 * self: the boss; the spit starts 46 px ahead and 7 px back, 64 px up, at speed $900 in x
 *       (the facing side) and y.
 * kind: kept in the record (StateSpitFire passes 0).
 * Nothing happens when 8 spits are already flying. */
#include "xmen2.h"

void BigBio_SpitSpawn(register BigBio *self, char kind)
{
	register BigBioSpit *spit;
	long dx;

	if (g_BigBioSpitCount < 8) {
		dx = self->facingRight ? (short)0x2E00 : (short)-0x2E00;
		spit = &g_BigBioSpitPool[g_BigBioSpitCount++];
		spit->x = self->x + dx + -0x700;
		spit->y = self->y + -0x4000;
		spit->bounces = 0;
		spit->kind = kind;
		spit->frame = 0;
		spit->vx = self->facingRight ? 0x900 : -0x900;
		spit->vy = 0x900;
	}
}
