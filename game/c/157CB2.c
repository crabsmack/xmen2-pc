/* MagneticGlob_HitCb - what a magnetic globe does to a body found at its point
 * (ObjList_ForEachAtPoint, MagneticGlob_Update): TakeDamage (class slot +$26) from the globe with
 * its hit type and attack, at the point.
 *
 * target: the object hit.  x, y: the point (pixels).  self: the globe.
 * Returns 1: one hit stops the search. */
#include "xmen2.h"

char MagneticGlob_HitCb(register Actor *target, short x, short y, register MagneticGlob *self)
{
	(*target->cls->method[8])(target, self, self->hitType, self->attack, x, y);	/* +$26 TakeDamage */
	return 1;
}
