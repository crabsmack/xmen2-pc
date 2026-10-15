/* MagnetoBoss_StateIntroStart - MAGNETOBOSS first state (set by Init): wait inside the wall at
 * the left of the arena (x -$28, y $118 px) with the camera held in zone 0, then the first
 * shake (MagnetoBoss_StateIntroShake1) in 60 frames.
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_StateIntroStart(register MagnetoBoss *self)
{
	self->x = -0x2800;
	self->y = 0x11800;
	AvalonA4_SetCameraZone(0, 0, 0xB0, 2, 2);
	SET_STATE_WAIT(self, MagnetoBoss_StateIntroShake1, 60);
	return self->flags & 8;
}
