/* XNpc_CalcKnockback - the speed and deceleration of a knockback slide over a distance.
 *
 * self:   the object (unused).
 * dist:   pixels, signed (the way it goes).
 * *speed: dist * 256 / 4 (8 fraction bits a frame);  *decel: -speed / 8 a frame, so the slide
 *         stops after 8 frames, about dist pixels away. */
#include "xmen2.h"

void XNpc_CalcKnockback(XNpc *self, short dist, register s16 *speed, s16 *decel)
{
	long v = (long)dist << 8;

	*speed = (short)(v >> 2);
	*decel = -*speed >> 3;
}
