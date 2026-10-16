/* CoreZap_Init - COREZAP constructor (class slot +$12): the parent's (ACTOR) Init, then flags6 |= 3
 * (out of the hit lists: it tests the players itself, CoreZap_Update) and not moving
 * (CoreShields_FireZap gives it its speed).
 *
 * self: the new zap.  x, y: position (8 fraction bits).  pl: placement entry (only passed on). */
#include "xmen2.h"

void CoreZap_Init(register CoreZap *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_CoreZap_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->flags6 |= 3;
	STOP_MOVING(self);
}
