/* LightBeam_Activate - LIGHTBEAM Activate (class slot +$3E): the parent's (ACTOR) Activate, then free
 * the per-object Timer it started: a beam is drawn but never updated.
 *
 * self: the beam. */
#include "xmen2.h"

void LightBeam_Activate(LightBeam *self)
{
	(*CLASS(Tbl_LightBeam_vtable)->parent->method[14])(self);	/* ACTOR Activate */
	Timer_Free((Timer *)self->timerObj);
	self->timerObj = 0;
}
