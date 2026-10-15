/* TechWallGun_StartCeiling - first state of a ceiling gun (TechWallGun_Init): aim down from the
 * next update (TechWallGun_StateCeilAimDown).
 *
 * self: the gun.
 * Returns flags bit 3. */
#include "xmen2.h"

short TechWallGun_StartCeiling(TechWallGun *self)
{
	SET_STATE(self, TechWallGun_StateCeilAimDown);
	return self->flags & 8;
}
