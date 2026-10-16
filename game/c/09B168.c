/* BroodClaw_Kill - BROODCLAW Kill (class slot +$52): kill its pair too (and forget it), then
 * ACTOR's Kill.
 *
 * self: the claw. */
#include "xmen2.h"

void BroodClaw_Kill(BroodClaw *self)
{
	if (self->pair) {
		(*self->pair->cls->method[19])(self->pair);		/* +$52 Kill */
		self->pair = 0;
	}
	(*CLASS(Tbl_BroodClaw_vtable)->parent->method[19])(self);	/* ACTOR Kill */
}
