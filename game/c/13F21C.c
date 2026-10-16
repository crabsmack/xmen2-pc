/* Pop_FxChainPath - POP state (fxType 4): become a chain of explosions along the path
 * Tbl_ScatterExplodeChain from its position, damaging when its attack is set.
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void Pop_FxChainPath(register Pop *self)
{
	ExplodeFx_Chain(self, CODEPTR(Tbl_ScatterExplodeChain), self->attack, 0, 0);
}
