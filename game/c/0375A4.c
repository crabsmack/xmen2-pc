/* MagnetoBoss_StateZone4 - MAGNETOBOSS in camera zone 4: while the wall at column $39 (row 10)
 * stands, shoot at the target each time the cooldown is over and change sides as the target
 * crosses the screen (moveMode 2 when it is left of camera x + $80, 3 when right of camera x +
 * $C0); once the wall is broken, free the camera (zone 4 cleared) and the x range, and leave
 * (MagnetoBoss_Zone4Exit). Nothing without a target.
 *
 * self: the boss.
 * Returns self->flags & 8 when the state changes after a delay, nothing otherwise. Its own x
 * in pixels is read before a shot and not used. */
#include "xmen2.h"

short MagnetoBoss_StateZone4(register MagnetoBoss *self)
{
	register Actor *t;
	register unsigned short wall;
	register short tx;
	register unsigned char mode;
	short x;

	if (t = self->target) {
		tx = t->x >> 8;
		mode = self->moveMode;
		wall = (g_FgBlockMap[g_FgRowTable[10] + 0x39] & 0x3800) >> 11;
		if (!wall) {
			AvalonA4_ClearCameraZone(4);
			self->bossFlags &= ~0xE;
			self->minX = -100;
			self->maxX = 8000;
			SET_STATE(self, MagnetoBoss_Zone4Exit);
			return self->flags & 8;
		}
		if (!self->shotCooldown) {
			x = self->x >> 8;
			self->shotCooldown = self->shotPeriod;
			if (self->shotDelay == -1)
				GOTO_STATE(self, MagnetoBoss_StateAimShoot);
			else {
				SET_STATE_WAIT(self, MagnetoBoss_StateAimShoot, self->shotDelay);
				return self->flags & 8;
			}
		} else if (mode == 2) {
			if (tx > g_CameraX + 0xC0) {
				self->bossFlags &= ~0xC;
				self->moveMode = 3;
			}
		} else if (mode == 3) {
			if (tx < g_CameraX + 0x80) {
				self->bossFlags &= ~0xC;
				self->moveMode = 2;
			}
		}
	}
	NO_RETURN_VALUE
}
