/* Launcher_StateFire - LAUNCHER state: fire a shot each way (Launcher_AddShot, +-shotSpeed) with
 * sound $BA (restarted), then frame 0 again from the next update (Launcher_StateFrame0).
 *
 * self: the launcher. Returns self->flags & 8. Never runs in the corpus. */
#include "xmen2.h"

short Launcher_StateFire(register Launcher *self)
{
	Launcher_AddShot(self, self->shotSpeed);
	Launcher_AddShot(self, -self->shotSpeed);
	Snd_Stop(0xBA);
	Snd_Play(0xBA);
	SET_STATE(self, Launcher_StateFrame0);
	return self->flags & 8;
}
