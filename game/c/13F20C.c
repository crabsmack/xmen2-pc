/* Pop_FxTrailArc - POP state (fxType 2): become an arc of explosion trails (ExplodeFx_TrailArc).
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void Pop_FxTrailArc(Pop *self)
{
	ExplodeFx_TrailArc(self);
}
