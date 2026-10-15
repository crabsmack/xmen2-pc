/* MagnetoBoss_StateZone1 - MAGNETOBOSS in camera zone 1: while the wall at column $16 (rows 8
 * and 9) stands, shoot at the target each time the cooldown is over and change sides as the
 * target crosses the screen (moveMode 2 when it is left of camera x + $80, 3 when right of
 * camera x + $C0); once the wall is broken, free the camera (zone 1 cleared) and the x range,
 * and leave (MagnetoBoss_Zone1Exit). Nothing without a target.
 *
 * self: the boss.
 * A map cell's bits 11-13 are 0 when its wall block is gone. Returns self->flags & 8 when the
 * state changes after a delay, nothing otherwise. */
#include "xmen2.h"

short MagnetoBoss_StateZone1(register MagnetoBoss *self)
{
	register Actor *t;
	register unsigned short wall8, wall9;
	register unsigned char mode;
	register short tx;

	if (t = self->target) {
		tx = t->x >> 8;
		mode = self->moveMode;
		wall8 = (g_FgBlockMap[g_FgRowTable[8] + 0x16] & 0x3800) >> 11;
		wall9 = (g_FgBlockMap[g_FgRowTable[9] + 0x16] & 0x3800) >> 11;
		if (!wall8 || !wall9) {
			AvalonA4_ClearCameraZone(1);
			self->bossFlags &= ~0xE;
			self->minX = -100;
			self->maxX = 8000;
			SET_STATE(self, MagnetoBoss_Zone1Exit);
			return self->flags & 8;
		}
		if (!self->shotCooldown) {
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
		} else if (tx < g_CameraX + 0x80) {
			self->bossFlags &= ~0xC;
			self->moveMode = 2;
		}
	}
	NO_RETURN_VALUE
}
