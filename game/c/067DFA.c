/* BoxInputActor_Move - BOXINPUTACTOR class slot +$4E: no movement, it only stops.
 *
 * self: the cursor. */
#include "xmen2.h"

void BoxInputActor_Move(register BoxInputActor *self)
{
	STOP_MOVING(self);
}
