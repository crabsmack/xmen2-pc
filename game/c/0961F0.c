/* XClone_StatePodWaitOpen - XCLONE entrance: wait until the pod has opened, then play its closing
 * animation (Tbl_BioBallsAnim, sound $F2; BioBalls_ActClearAnim at its end) and emerge
 * (XClone_StateEmerge).
 *
 * self: the clone. Returns flags & 8. */
#include "xmen2.h"

short XClone_StatePodWaitOpen(XClone *self)
{
	BioBalls *pod = self->child;

	if (!BioBalls_IsDone(pod)) {
		SET_STATE(self, XClone_StatePodWaitOpen);
		return self->flags & 8;
	}
	pod->done = 0;
	Snd_Stop(0xF2);
	Snd_Play(0xF2);
	Obj_SetAnim(pod, CODEPTR(Tbl_BioBallsAnim));
	pod->state = (StateFn)BioBalls_ActClearAnim;
	pod->timer = 0xFF;
	SET_STATE(self, XClone_StateEmerge);
	return self->flags & 8;
}
