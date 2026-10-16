/* Spark_StateStop - SPARK state: stop, and dart again (Spark_StateDart) in 10 frames.
 *
 * self: the object.
 * Returns flags bit 3. Never runs in the corpus. */
#include "xmen2.h"

short Spark_StateStop(register Spark *self)
{
	STOP_MOVING(self);
	SET_STATE_WAIT(self, Spark_StateDart, 10);
	return self->flags & 8;
}
