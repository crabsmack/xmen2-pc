/* Apocalypse_Move - APOCALYPSE Move (class slot +$4E; replaces the parent's).
 *
 * self: the boss.  steps: frames to move (at least one is).
 * Circling (moveFlags bit 0): placed on the wide ellipse x = 256 - 175 sin a, y = 133 + 10 cos a
 * (once, whatever steps is), then the angle advances by angleStep (twice that in two-player
 * step mode), wrapping at 360, and the frame follows the angle: 0 below 100, $81 below 125, $82
 * below 150, $82 below 235, $81 below 260, else 0 ($83 below 30 can't happen: kept).
 * Otherwise, each step: while moving (bit 4) count down to the hover wobble
 * (Apocalypse_StartJitter) and stop within 16 px (|dx| + |dy|) of the target; integrate speed and
 * acceleration; with the wave wobble (bit 6) add the wave table's offsets (x at wavePhase, y
 * waveYPhase ahead, >> waveShift) and advance the phase. */
#include "xmen2.h"

void Apocalypse_Move(register Apocalypse *self, short steps)
{
	if (self->moveFlags & 1) {
		register short dx, dy;
		register short frame;

		dx = Math_SinDeg(self->angle) * 175 >> 16;
		dy = Math_CosDeg(self->angle) * 10 >> 16;
		self->x = (256 - dx) << 8;
		self->y = (dy + 0x85) << 8;
		self->angle += g_TwoPlayerStep ? self->angleStep << 1 : self->angleStep;
		if (self->angle >= 360)
			self->angle -= 360;
		else if (self->angle < 0)
			self->angle += 360;
		if (self->angle < 100)
			frame = 0;
		else if (self->angle < 125)
			frame = 0x81;
		else if (self->angle < 150)
			frame = 0x82;
		else if (self->angle < 30)
			frame = 0x83;
		else if (self->angle < 235)
			frame = 0x82;
		else if (self->angle < 260)
			frame = 0x81;
		else
			frame = 0;
		(*self->cls->method[20])(self, frame);		/* +$56 SetFrame */
	} else {
		do {
			if (self->moveFlags & 0x10) {
				if (self->jitterDelay != -100) {
					if (self->jitterDelay <= 0)
						Apocalypse_StartJitter(self);
					else
						self->jitterDelay--;
				}
				if (ABS(self->targetX - (self->x >> 8)) + ABS(self->targetY - (self->y >> 8)) <= 16) {
					STOP_MOVING(self);
					self->moveFlags &= ~0x10;
				}
			}
			self->x += self->vx;
			self->y += self->vy;
			self->vx += self->ax;
			self->vy += self->ay;
			if (self->moveFlags & 0x40) {
				register unsigned char phase = self->wavePhase;
				unsigned char yphase = phase + self->waveYPhase & 0x1F;
				register short shift = self->waveShift;
				register long wx = self->wave[phase] >> shift;
				register long wy = self->wave[yphase] >> shift;

				self->x += wx;
				self->y += wy;
				self->wavePhase++;
				self->wavePhase = self->wavePhase & 0x1F;
			}
		} while (--steps);
	}
}
