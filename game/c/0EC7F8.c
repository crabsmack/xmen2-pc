/* CoreElevator_Reset - class slot +$6A of COREELEVATOR: does nothing (calls the empty
 * CoreElevator_EmptyStub).
 *
 * self: the object; unused. */
#include "xmen2.h"

void CoreElevator_Reset(CoreElevator *self)
{
	CoreElevator_EmptyStub();
}
