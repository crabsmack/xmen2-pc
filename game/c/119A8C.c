/* Synchronizer_WaitMembers - SYNCHRONIZER state, members deactivated: when any is back in the
 * activation window (its slot +$6E, SyncActor_MemberInWindow) activate them all next frame
 * (Synchronizer_ActivateMembers); when every member's spawn record is out of view (slot +$76,
 * SyncActor_MemberSpawnRecOutOfView) despawn them (Synchronizer_DespawnMembers).
 *
 * self: the synchronizer.
 * A member in the window isn't tested for its record; the despawn test comes last and wins. */
#include "xmen2.h"

void Synchronizer_WaitMembers(register Synchronizer *self)
{
	register short out = 0;
	register short i;
	register Actor *m;

	for (i = 0; i < self->memberCount; i++) {
		m = self->member[i];
		if ((char)(*m->cls->method[26])(m))			/* +$6E MemberInWindow */
			self->state = Synchronizer_ActivateMembers;
		else if ((char)(*m->cls->method[28])(m, &g_SpawnRecs[m->spawnRec]))	/* +$76 MemberSpawnRecOutOfView */
			out++;
	}
	if (out == self->memberCount)
		self->state = Synchronizer_DespawnMembers;
}
