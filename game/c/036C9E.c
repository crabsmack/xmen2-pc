/* MagnetoBoss_StateIntroWait1 - MAGNETOBOSS intro: once the shake is over, the second shake
 * (MagnetoBoss_StateIntroShake2) in 60 frames.
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes, nothing while it waits. */
#include "xmen2.h"

short MagnetoBoss_StateIntroWait1(register MagnetoBoss *self)
{
	if (!g_CamShake) {
		SET_STATE_WAIT(self, MagnetoBoss_StateIntroShake2, 60);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
