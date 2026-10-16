/* RThetaObject_Init - RTHETAOBJECT Init (class slot +$12): the parent's (ACTOR) Init, then polar
 * motion at rest: no gravity, speed and acceleration 0, maximum speed $1000, heading 0 (east), no
 * turning; flags bit $4000 cleared.
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry or 0. */
#include "xmen2.h"

void RThetaObject_Init(RThetaObject *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_RThetaObject_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->ay = 0;
	self->speed = 0;
	self->accel = 0;
	self->maxSpeed = 0x1000;
	self->heading = 0;
	self->turnRate = 0;
	self->turnAccel = 0;
	self->flags &= ~0x4000;
}
