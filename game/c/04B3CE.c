/* Apocalypse_MoveTo - APOCALYPSE: start a damped move to (x, y): an acceleration from half the
 * distance and the current speed, both scaled by the shift, then Apocalypse_Move flies it there
 * and ends the move (moveFlags bit 4) within 16 px.
 *
 * self: the boss.  x, y: the target (8 fraction bits).  shift: larger is slower (4 or 5).
 * acceleration = (target - position) / 2 >> (2 shift - 1), minus speed >> shift (each axis);
 * the target is kept in pixels; the hover wobble starts after 1 << shift steps. */
#include "xmen2.h"

void Apocalypse_MoveTo(register Apocalypse *self, register long x, register long y, register long shift)
{
	long dx = (x - self->x) >> 1;
	long dy = (y - self->y) >> 1;
	register long shift2 = shift + shift - 1;
	short ax = (dx >> shift2) - (self->vx >> shift);
	short ay = (dy >> shift2) - (self->vy >> shift);

	self->ax = ax;
	self->ay = ay;
	self->targetX = x >> 8;
	self->targetY = y >> 8;
	self->moveFlags |= 0x10;
	self->jitterDelay = 1 << shift;
}
