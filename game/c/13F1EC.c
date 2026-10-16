/* Pop_FxBurstSmall - POP state (fxType 3): become a small explosion burst (ExplodeFx_BurstSmall).
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void Pop_FxBurstSmall(Pop *self)
{
	ExplodeFx_BurstSmall(self);
}
