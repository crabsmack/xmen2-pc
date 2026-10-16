/* Pop_FxBounce - POP state (fxType 1): become bouncing explosion puffs (ExplodeFx_Bounce).
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void Pop_FxBounce(Pop *self)
{
	ExplodeFx_Bounce(self);
}
