/* Pop_FxBurstMedium - POP state (fxType 0 and others): become a medium explosion burst
 * (ExplodeFx_BurstMedium).
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void Pop_FxBurstMedium(Pop *self)
{
	ExplodeFx_BurstMedium(self);
}
