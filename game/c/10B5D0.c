/* Elevator_Update - ELEVATOR's per-frame update (class slot +$0E; SpaceA1_FrameHook): unless
 * stopped, the speed takes the acceleration (kept within -$900..$900, acceleration then 0, while
 * the scene isn't ending) and moves the position (speed >> 8). Low in the shaft (position below
 * $C00) going down, the camera shakes (g_CamShake = Tbl_ElevatorShake, the shake script, when no
 * shake runs); otherwise no shake. The toggle delay counts down to 0.
 *
 * self: the lift. */
#include "xmen2.h"

void Elevator_Update(register Elevator *self)
{
	if (!self->stopped) {
		self->prevSpeed = self->speed;
		self->speed += self->accel;
		if (!g_SceneExitTimer) {
			if (self->speed > 0x900) {
				self->speed = 0x900;
				self->accel = 0;
			} else if (self->speed < -0x900) {
				self->speed = -0x900;
				self->accel = 0;
			}
		}
		self->pos += self->speed >> 8;
	}
	if (self->pos < 0xC00) {
		if (self->up)
			g_CamShake = 0;
		else if (!g_CamShake)
			g_CamShake = CODEPTR(Tbl_ElevatorShake);
	} else
		g_CamShake = 0;
	if (self->toggleDelay > 0) {
		--self->toggleDelay;
		if (self->toggleDelay < 0)
			self->toggleDelay = 0;
	}
}
