/* ScalingActor_Move - SCALINGACTOR class slot +$4E: no movement, it only stops (the states set its
 * position).
 *
 * self: the portrait. */
#include "xmen2.h"

void ScalingActor_Move(register ScalingActor *self)
{
	STOP_MOVING(self);
}
