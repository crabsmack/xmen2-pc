/* Ninja_Deactivate - NINJA Deactivate (class slot +$46): a ninja running its leave script
 * (Tbl_NinjaLeaveScript) is gone for good (Actor_DeactivateFree); any other, the parent's
 * (BIONPC) Deactivate.
 *
 * self: the object. */
#include "xmen2.h"

void Ninja_Deactivate(register Ninja *self)
{
	if (self->ai.table == CODEPTR(Tbl_NinjaLeaveScript))
		Actor_DeactivateFree(self);
	else
		(*CLASS(Tbl_Ninja_vtable)->parent->method[16])(self);	/* BIONPC Deactivate */
}
