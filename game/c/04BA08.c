/* Apocalypse_StateOrbitStep - APOCALYPSE state: a damped move (shift 5) to the point at its
 * circling angle on the wide ellipse x = 256 - 175 sin a, y = 133 + 10 cos a; then
 * Apocalypse_StateWaitArrive.
 *
 * self: the boss. Returns self->flags & 8.
 * Math_SinDeg/Math_CosDeg are 16.16 fixed point. */
#include "xmen2.h"

short Apocalypse_StateOrbitStep(register Apocalypse *self)
{
	register short dy;
	register long x;
	register short dx;
	register long y;

	dx = Math_SinDeg(self->angle) * 175 >> 16;
	dy = Math_CosDeg(self->angle) * 10 >> 16;
	x = (256 - dx) << 8;
	y = (dy + 0x85) << 8;
	Apocalypse_MoveTo(self, x, y, 5);
	SET_STATE(self, (StateFn)Apocalypse_StateWaitArrive);
	return self->flags & 8;
}
