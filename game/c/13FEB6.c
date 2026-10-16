/* Spark_StateStart - SPARK state (Init's, and after a lap along its segments): its animation
 * (Tbl_SparkAnim), then, in mode 1 (ACTOR unk_8A[1]), onto the nearest floor segment
 * (Obj_SnapToNearestSeg(self, 1000, -1)) at speed both ways, flying along it
 * (Spark_StateFlyStraight); in any other mode it targets player 1 and hops at it
 * (Spark_StateStop). Either from the next update.
 *
 * self: the object.
 * Returns flags bit 3. Never runs in the corpus. */
#include "xmen2.h"

short Spark_StateStart(register Spark *self)
{
	Obj_SetAnim(self, CODEPTR(Tbl_SparkAnim));
	if ((char)self->unk_8A[1] == 1) {
		Obj_SnapToNearestSeg(self, 0x3E8, -1);
		self->vx = self->speed;
		self->vy = self->speed;
		SET_STATE(self, Spark_StateFlyStraight);
		return self->flags & 8;
	} else {
		self->target = (Actor *)&g_Player1;
		SET_STATE(self, Spark_StateStop);
		return self->flags & 8;
	}
}
