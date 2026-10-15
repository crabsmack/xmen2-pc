/* MagnetoBoss_StateIntroWait3 - MAGNETOBOSS intro: once the shake is over, shake and crack
 * again (MagnetoBoss_StateIntroShake3) in 40 frames.
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes, nothing while it waits. */
#include "xmen2.h"

short MagnetoBoss_StateIntroWait3(register MagnetoBoss *self)
{
	if (!g_CamShake) {
		SET_STATE_WAIT(self, MagnetoBoss_StateIntroShake3, 40);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
