/* MagneticGlob_New - MAGNETICGLOB New (class slot +$0A): Magneto has one globe, the static
 * g_MagneticGlobObj: a globe still out is killed, then the object becomes one of class cls (its
 * Init, slot +$12) and is put into play (Activate, slot +$3E).
 *
 * unused: 0 (New's object argument).  cls: the class descriptor.  x, y: position (8 fraction
 * bits).  pl: placement params (Magneto_SpawnShot: by player, priority, VRAM tiles).
 * Returns the globe. */
#include "xmen2.h"

MagneticGlob *MagneticGlob_New(void *unused, Class *cls, long x, long y, Placement *pl)
{
	register MagneticGlob *self = &g_MagneticGlobObj;

	if (self->flags & 0x8000)
		(*self->cls->method[19])(self);			/* +$52 Kill */
	self->cls = cls;
	(*self->cls->method[3])(self, x, y, pl);		/* +$12 Init */
	(*self->cls->method[14])(self);				/* +$3E Activate */
	return self;
}
