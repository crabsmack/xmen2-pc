/* BoxInputActor_OutsideActiveWindow - BOXINPUTACTOR class slot +$42: never outside the active
 * window (ACTOR's tests the screen bounds), so the cursor is never deactivated.
 *
 * self: the cursor (unused). Returns 0. */
#include "xmen2.h"

long BoxInputActor_OutsideActiveWindow(BoxInputActor *self)
{
	return 0;
}
