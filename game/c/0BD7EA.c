/* Barbarian_TargetPlayer - if player p is alive and stands on one of the segments BARBARIAN can
 * reach (its neighbours list), make p the target: target, targetSeg, and the point to go to,
 * p's position moved 20 px on in the direction BARBARIAN faces (Barbarian_StepTargetX).
 *
 * self: the object.  p: g_Player1 or g_Player2.
 * Returns 1 when p became the target, else 0. */
#include "xmen2.h"

char Barbarian_TargetPlayer(Barbarian *self, XPlayer *p)
{
	Seg *seg;
	u8 n;
	long i;

	if (IS_ALIVE(p)) {
		for (i = 0; i < 12; i++) {
			n = self->neighbours[i];
			if (n == 0xFF)
				return 0;
			seg = &g_Segs[n];
			if (seg == p->seg) {
				self->target = p;
				self->targetSeg = seg;
				self->targetX = p->x >> 8;
				self->targetY = p->y >> 8;
				Barbarian_StepTargetX(self, seg, 20);
				return 1;
			}
		}
	}
	return 0;
}
