/* ExplodeFx_TrailArc - death effect: a Trail whose head shoots up or down (vy $800, from where
 * the object is: down above y $70, up below it) and curves back (ay -$20 / $20), drifting
 * towards x $A0 (ax $200 / -$200).
 *
 * obj: the dying object (killed).
 * The tests use its map position in pixels, not its screen position. */
#include "xmen2.h"

void ExplodeFx_TrailArc(void *obj)
{
	register Actor *o = (Actor *)obj;
	register short x = o->x >> 8;
	register short y = o->y >> 8;
	register short vx, vy, ax, ay;

	if (x < 0xA0) {
		vx = 0;
		ax = 0x200;
	} else {
		vx = 0;
		ax = -0x200;
	}
	if (y < 0x70) {
		vy = 0x800;
		ay = -0x20;
	} else {
		vy = -0x800;
		ay = 0x20;
	}
	ExplodeFx_Trail(o, vx, vy, ax, ay);
}
