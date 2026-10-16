/* TankLeft_Deactivate - TANKLEFT Deactivate (class slot +$46): a tank driving away (its script
 * is Tbl_Tank_ScriptDrive) that leaves the active window is gone for good (Actor_DeactivateFree);
 * otherwise the parent's (TANK) Deactivate.
 *
 * self: the object. */
#include "xmen2.h"

void TankLeft_Deactivate(register TankLeft *self)
{
	if (self->ai.table == CODEPTR(Tbl_Tank_ScriptDrive))
		Actor_DeactivateFree(self);
	else
		(*CLASS(Tbl_TankLeft_vtable)->parent->method[16])(self);	/* TANK Deactivate */
}
