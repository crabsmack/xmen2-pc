/* XClone_DieRestartMusic - XCLONE death step: restart the stage music ($5F) and play the bouncing
 * puffs death effect (ExplodeFx_Bounce) on the clone.
 *
 * self: the clone (a script action). */
#include "xmen2.h"

void XClone_DieRestartMusic(XClone *self)
{
	Music_PlayStage(0x5F);
	ExplodeFx_Bounce(self);
}
