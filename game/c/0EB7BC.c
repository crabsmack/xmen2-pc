/* CoreShields_Reset - class slot +$6A of CORESHIELDS: does nothing (calls the empty
 * CoreShields_EmptyStub). CoreShields_ClassInit calls it too.
 *
 * self: the object (0 from CoreShields_ClassInit); unused. */
#include "xmen2.h"

void CoreShields_Reset(CoreShields *self)
{
	CoreShields_EmptyStub();
}
