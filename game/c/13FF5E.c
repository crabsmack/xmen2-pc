/* Spark_StateDart - SPARK state: dart for 40 frames at a point near its target, then stop
 * (Spark_StateStop).
 *
 * self: the object (target set by Spark_StateStart).
 * The point: the target's position 40 px up, each coordinate moved by a random draw >> 10
 * (negated when the draw is even, so up to about 32 px either way). The velocity is a quarter of
 * the way there, halved (both) until |vx| + |vy| is at most $128. The velocity copied into two
 * variables that are never read is kept. Returns flags bit 3. Never runs in the corpus. */
#include "xmen2.h"

short Spark_StateDart(register Spark *self)
{
	register long tx = self->target->x;
	register long ty = self->target->y - 0x2800;
	register short r;

	r = Rand_Next();
	tx += (r & 1 ? r : -r) >> 10;
	r = Rand_Next();
	ty += (r & 1 ? r : -r) >> 10;
	self->vx = (short)((tx - self->x) >> 2);
	self->vy = (short)((ty - self->y) >> 2);
	{
		register short vx = self->vx;
		register short vy = self->vy;

		while (ABS(self->vx) + ABS(self->vy) > 0x128) {
			self->vx = self->vx >> 1;
			self->vy = self->vy >> 1;
		}
		if (0) {
			(void)vx;
			(void)vy;
		}
	}
	SET_STATE_WAIT(self, Spark_StateStop, 0x28);
	return self->flags & 8;
}
