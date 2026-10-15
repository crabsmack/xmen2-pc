/* XClone_Move - move XCLONE (class slot +$4E): integrate velocity and acceleration `steps` times,
 * then keep it inside its arena.
 *
 * self:  the clone.
 * steps: frames to integrate (at least one is).
 * At the left/right edge the horizontal motion stops; below the floor it stands on the floor
 * (vertical speed and gravity cleared); above the ceiling only the vertical speed is cleared.
 * The original looped with a label: MPW C then gives steps a register of its own (a do/while
 * shares px's or py's). */
#include "xmen2.h"

void XClone_Move(XClone *self, short steps)
{
	short px, py;

again:
	self->x += self->vx;
	self->y += self->vy;
	self->vx += self->ax;
	self->vy += self->ay;
	steps--;
	if (steps > 0)
		goto again;
	px = self->x >> 8;
	py = self->y >> 8;
	if (px < self->arenaLeft) {
		self->ax = 0;
		self->vx = 0;
		self->x = self->arenaLeft << 8;
	} else if (px > self->arenaRight) {
		self->ax = 0;
		self->vx = 0;
		self->x = self->arenaRight << 8;
	}
	if (py > self->arenaFloor) {
		self->vy = 0;
		self->ay = 0;
		self->y = self->arenaFloor << 8;
	} else if (py < self->arenaTop) {
		self->vy = 0;
		self->y = self->arenaTop << 8;
	}
}
