/* CoreShields_Update - CORESHIELDS Update (class slot +$4A): the parent's (ACTOR) Update, then the
 * zap cooldown runs down by the frame's steps (2 in two-player step mode, else 1).
 *
 * self: the shields. */
#include "xmen2.h"

void CoreShields_Update(register CoreShields *self)
{
	(*CLASS(Tbl_CoreShields_vtable)->parent->method[17])(self);	/* ACTOR Update */
	if (self->zapCooldown > 0)
		self->zapCooldown -= g_TwoPlayerStep ? 2 : 1;
}
