/* BrainRocket_Deactivate - BRAINROCKET Deactivate (class slot +$46): gone for good
 * (Actor_DeactivateFree: no respawn).
 *
 * self: the rocket. */
#include "xmen2.h"

void BrainRocket_Deactivate(BrainRocket *self)
{
	Actor_DeactivateFree(self);
}
