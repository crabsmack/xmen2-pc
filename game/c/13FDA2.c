/* Spark_Move - SPARK Move (class slot +$4E; the parent's isn't called): a free spark (ACTOR
 * unk_8A[1] 0) drifts its origin by its velocity and sits at the origin plus the current step
 * of Tbl_SparkWobble (pixels; the next step every other frame, g_FrameCounter bit 0; a dx of
 * $100 or more restarts the table); any other rides its rail (Obj_RideRail).
 *
 * self: the object.  steps: frames to move (1, or 2 in the two-player step mode; the rail's).
 * Never runs in the corpus. */
#include "xmen2.h"

void Spark_Move(register Spark *self, short steps)
{
	register SparkWobbleStep *w;

	if (!self->unk_8A[1]) {
		w = (SparkWobbleStep *)CODEPTR(Tbl_SparkWobble);
		if ((w + self->wobble)->dx >= 0x100)
			self->wobble = 0;
		w += self->wobble;
		self->originX += self->vx;
		self->originY += self->vy;
		self->x = self->originX + ((long)w->dx << 8);
		self->y = self->originY + ((long)w->dy << 8);
		if ((g_FrameCounter & 1) != 0)
			self->wobble++;
	} else
		Obj_RideRail(self, steps);
}
