/* XClone_StateDropPod - XCLONE entrance: hide the clone, take it off the enemy body hit list and
 * drop a pod (a BIOBALLS object, reused or new) from the arena ceiling above it (vy $100,
 * gravity $80); then XClone_StatePodFall. Without a pod it emerges without the fall
 * (XClone_StateEmerge from the next update).
 *
 * self: the clone (state set by XClone_StateWaitPlayers).
 * Returns flags & 8. */
#include "xmen2.h"

short XClone_StateDropPod(XClone *self)
{
	BioBalls *pod;

	self->anim = 0;
	(*self->cls->method[20])(self, 0xFF);		/* SetFrame: none */
	if (self->child)
		pod = self->child;
	else
		pod = (BioBalls *)(*CLASS(Tbl_BioBalls_vtable)->method[2])(0,	/* New */
			CLASS(Tbl_BioBalls_vtable), self->x, self->y, 0);
	if (pod) {
		self->flags6 |= 2;
		List_Unlink(self->bodyNode, &g_HitListEnemyBody);
		self->bodyNode = 0;
		pod->x = self->x;
		pod->y = self->arenaTop << 8;
		BioBalls_ResetToPos(pod);
		pod->done = 0;
		pod->state = (StateFn)BioBalls_StateIdle;
		pod->timer = 0xFF;
		STOP_MOVING(pod);
		pod->vy = 0x100;
		pod->ay = 0x80;
		self->child = pod;
		SET_STATE(self, XClone_StatePodFall);
		return self->flags & 8;
	}
	SET_STATE(self, XClone_StateEmerge);
	return self->flags & 8;
}
