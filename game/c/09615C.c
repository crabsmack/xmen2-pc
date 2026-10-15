/* XClone_StatePodOpen - XCLONE entrance: open the pod at the clone's position, facing its way: pod
 * script XClone_PodScript, sound $EF, its balls spread from one point along the clone record's
 * ball path (BioBalls_WaitPathDone); then XClone_StatePodWaitOpen.
 *
 * self: the clone. Returns flags & 8. */
#include "xmen2.h"

short XClone_StatePodOpen(XClone *self)
{
	BioBalls *pod = self->child;

	if (!(self->spr.flags & 0x10))
		pod->spr.flags &= ~0x10;
	else
		pod->spr.flags |= 0x10;
	AiScript_Set(&pod->ai, CODEPTR(Tbl_XClone_PodScriptLen));
	pod->x = self->x;
	pod->y = self->y;
	pod->done = 0;
	Snd_Stop(0xEF);
	Snd_Play(0xEF);
	BioBalls_SetPathCollapsed(pod, CLONE_PART(self->record, ballPath));
	pod->state = (StateFn)BioBalls_WaitPathDone;
	pod->timer = 0;
	SET_STATE(self, XClone_StatePodWaitOpen);
	return self->flags & 8;
}
