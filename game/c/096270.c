/* XClone_StateEmerge - XCLONE entrance: once the pod is done, remove it; the clone becomes
 * hittable (enemy body hit list), stands and starts its script from the first step, now.
 *
 * self: the clone (after XClone_StatePodWaitOpen, or XClone_StateDropPod without a pod).
 * Returns flags & 8 while waiting; nothing in particular when the script starts. */
#include "xmen2.h"

short XClone_StateEmerge(XClone *self)
{
	if (self->child) {
		if (!BioBalls_IsDone(self->child)) {
			SET_STATE(self, XClone_StateEmerge);
			return self->flags & 8;
		}
		(*self->child->cls->method[19])(self->child);	/* Kill */
		self->child = 0;
	}
	self->flags6 &= ~2;
	self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
	Obj_ChangeAnim(self, CLONE_PART(self->record, standAnim));
	AI_FIRST_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
