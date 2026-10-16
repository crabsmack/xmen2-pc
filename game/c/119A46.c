/* Synchronizer_ActivateMembers - SYNCHRONIZER state, a member came back into the window:
 * activate every member (slot +$3E), then run them (Synchronizer_RunMembers).
 *
 * self: the synchronizer. */
#include "xmen2.h"

void Synchronizer_ActivateMembers(register Synchronizer *self)
{
	register short i;
	register Actor *m;

	for (i = 0; i < self->memberCount; i++) {
		m = self->member[i];
		(*m->cls->method[14])(m);		/* +$3E Activate */
	}
	self->state = Synchronizer_RunMembers;
}
