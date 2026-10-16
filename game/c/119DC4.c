/* SyncActor_InActivateWindow - SYNCHRONIZEDACTOR slot +$3A: a member never activates on its own.
 * When it is in the activation window (slot +$6E, SyncActor_MemberInWindow) and its group isn't
 * running yet (slot +$7A, SyncActor_IsTriggered), it starts the group's SYNCHRONIZER (New: its
 * class, its board entry), which gathers and activates the members.
 *
 * self: the object.  Returns 0. */
#include "xmen2.h"

char SyncActor_InActivateWindow(register Synchronizedactor *self)
{
	register char in;

	if (in = (char)(*self->cls->method[26])(self)) {		/* +$6E MemberInWindow */
		if (!(short)(*self->cls->method[29])(self))		/* +$7A IsTriggered */
			(*CLASS(Tbl_Synchronizer_vtable)->method[1])(CLASS(Tbl_Synchronizer_vtable),	/* New */
				self->cls, self->classParam);
	}
	return 0;
}
