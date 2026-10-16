/* ScalingActor_OutsideActiveWindow - SCALINGACTOR class slot +$42: never outside the active window
 * (ACTOR's tests the screen bounds), so the portrait is never deactivated.
 *
 * self: the portrait (unused). Returns 0. */
#include "xmen2.h"

long ScalingActor_OutsideActiveWindow(ScalingActor *self)
{
	return 0;
}
