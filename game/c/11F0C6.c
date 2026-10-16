/* BioBalls_ActClearAnim - BIOBALLS script action (the end of BioBalls_StateAnimA/B and of the
 * XCLONE pod animations): no animation, then the next step on the next update.
 *
 * self: the object. */
#include "xmen2.h"

void BioBalls_ActClearAnim(BioBalls *self)
{
	self->anim = 0;
	AI_NEXT_STEP(self, self->ai, 0);
}
