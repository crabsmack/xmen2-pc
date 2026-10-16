/* Synchronizer_RunMembers - SYNCHRONIZER state, members active: update every member (slot +$4A);
 * when all of them are outside the active window (slot +$72, SyncActor_MemberOutsideWindow)
 * deactivate them next frame (Synchronizer_DeactivateMembers).
 *
 * self: the synchronizer. */
#include "xmen2.h"

void Synchronizer_RunMembers(register Synchronizer *self)
{
	register short out = 0;
	register short i;
	register Actor *m;

	for (i = 0; i < self->memberCount; i++) {
		m = self->member[i];
		(*m->cls->method[17])(m);			/* +$4A Update */
		if ((char)(*m->cls->method[27])(m))		/* +$72 MemberOutsideWindow */
			out++;
	}
	if (out == self->memberCount)
		self->state = Synchronizer_DeactivateMembers;
}
