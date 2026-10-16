/* Spark_StateFlyStraight - SPARK state (mode 1): fly along its floor segments; when the next
 * 8 px would leave the path's end (Seg_WouldLeaveEnd), back to its placement position and start
 * over (Spark_StateStart). Either from the next update.
 *
 * self: the object.
 * Returns flags bit 3. Never runs in the corpus. */
#include "xmen2.h"

short Spark_StateFlyStraight(register Spark *self)
{
	if (Seg_WouldLeaveEnd(self, 8)) {
		self->x = (long)self->px << 8;
		self->y = (long)self->py << 8;
		SET_STATE(self, Spark_StateStart);
		return self->flags & 8;
	} else {
		SET_STATE(self, Spark_StateFlyStraight);
		return self->flags & 8;
	}
}
