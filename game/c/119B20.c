/* Synchronizer_DeactivateMembers - SYNCHRONIZER state, all members out of the active window:
 * deactivate every member (slot +$46), then wait (Synchronizer_WaitMembers).
 *
 * self: the synchronizer. */
#include "xmen2.h"

void Synchronizer_DeactivateMembers(register Synchronizer *self)
{
	register short i;
	register Actor *m;

	for (i = 0; i < self->memberCount; i++) {
		m = self->member[i];
		(*m->cls->method[16])(m);		/* +$46 Deactivate */
	}
	self->state = Synchronizer_WaitMembers;
}
