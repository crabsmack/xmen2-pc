/* Pop_FxChainSpiral - POP state (fxType 5): become a harmless spiral of explosions
 * (Tbl_ExplodeFxSpiralPath) from the top of its body box.
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void Pop_FxChainSpiral(register Pop *self)
{
	ExplodeFx_Chain(self, CODEPTR(Tbl_ExplodeFxSpiralPath), 0, 0, self->boxTop - (self->y >> 8));
}
