/* MagnetoBoss_Move - MAGNETOBOSS Move (class slot +$4E): the parent's (ACTOR) Move, then its
 * wobble and the speed its moveMode asks for.
 *
 * self: the boss.
 * n:    steps (1, or 2 in two-player mode).
 * Wobble (bossFlags bit 0), n times: x and y move by entries of its wave table (x at wavePhase,
 * y at wavePhase + waveYPhase, both mod 32) shifted right by waveShift; wavePhase steps on.
 * moveMode (the speed is used by the next Move):
 *   2 hover beside the target: towards its box centre + (sideX, sideY) (8 fraction bits),
 *     that x kept in minX..maxX when bossFlags bit 1 (bit 2 / 3: it was clamped); speed =
 *     distance from its own box centre / 30 in x, / 55 in y. 3: the same with sideX mirrored.
 *     Nothing without a target.
 *   1 fly to (destX, destY): within 12 px (|dx| + |dy|) it stops (moveMode 0); else the speed
 *     is the distance / 10 in x and / 25 in y; when |vx| > $400 it becomes $400 (always
 *     positive, a bug: going left it turns round) and vy = vy * $400 / vx.
 *   4 drift to (destX, destY): speed = distance / 5.
 * Then, when bossFlags bit 1, x is clamped to minX..maxX (setting bit 2 / 3).
 * The cases are MagnetoBoss_Move_part1 ($03653E, case 3 first: the source order) and the
 * clamp is MagnetoBoss_Move_part2 ($0366E6) in the disassembly. */
#include "xmen2.h"

void MagnetoBoss_Move(register MagnetoBoss *self, short n)
{
	unsigned char yPhase;
	short count = n;
	long sx;

	(*CLASS(Tbl_MagnetoBoss_vtable)->parent->method[18])(self, n);	/* ACTOR Move */
	if (self->bossFlags & 1) {
		register unsigned char phase;
		register short shift;
		register long ox, oy;

		do {
			phase = self->wavePhase;
			yPhase = phase + self->waveYPhase & 0x1F;
			shift = self->waveShift;
			ox = self->wave[phase] >> shift;
			oy = self->wave[yPhase] >> shift;
			self->x += ox;
			self->y += oy;
			++self->wavePhase;
			self->wavePhase = self->wavePhase & 0x1F;
		} while (--count);
	}
	if (self->moveMode) {
		sx = self->sideX << 8;
		switch (self->moveMode) {
		default:
			break;
		case 3:		/* MagnetoBoss_Move_part1 */
			sx = -sx;
		case 2: {
			Actor *t;
			long min, max, tx, ty, dx, dy;

			if (!(t = self->target))
				break;
			min = self->minX << 8;
			max = self->maxX << 8;
			tx = ((t->boxRight + t->boxLeft) << 7) + sx;
			ty = ((t->boxTop + t->boxBottom) << 7) + (self->sideY << 8);
			if (self->bossFlags & 2) {
				if (tx < min) {
					tx = min;
					self->bossFlags |= 4;
				}
				if (tx > max) {
					tx = max;
					self->bossFlags |= 8;
				}
			}
			dx = tx - ((self->boxRight + self->boxLeft) << 7);
			dy = ty - ((self->boxTop + self->boxBottom) << 7);
			self->vx = dx / 30;
			self->vy = dy / 55;
			break;
		}
		case 1: {
			long dx, dy, vx, vy, nvx, nvy;

			dx = (self->destX << 8) - self->x;
			dy = (self->destY << 8) - self->y;
			if ((ABS(dx) + ABS(dy)) >> 8 <= 12) {
				STOP_MOVING(self);
				self->moveMode = 0;
				break;
			}
			vx = dx / 10;
			vy = dy / 25;
			if (ABS(vx) > 0x400) {
				nvx = 0x400;
				nvy = vy * 0x400 / vx;
				vx = nvx;
				vy = nvy;
			}
			self->vx = vx;
			self->vy = vy;
			break;
		}
		case 4: {
			long dx, dy;

			dx = (self->destX << 8) - self->x;
			dy = (self->destY << 8) - self->y;
			self->vx = dx / 5;
			self->vy = dy / 5;
		}
		}
	}
	if (self->bossFlags & 2) {	/* MagnetoBoss_Move_part2 */
		long min, max;

		min = self->minX << 8;
		max = self->maxX << 8;
		if (self->x < min) {
			self->x = min;
			self->bossFlags |= 4;
		}
		if (self->x > max) {
			self->x = max;
			self->bossFlags |= 8;
		}
	}
}
