/* XClone_ActCloseBioBalls - XCLONE script action: remove its BIOBALLS object (sound $F2) and go
 * back to the standing animation; go on with the script on the next update.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActCloseBioBalls(XClone *self)
{
	if (self->child) {
		Snd_Stop(0xF2);
		Snd_Play(0xF2);
		(*self->child->cls->method[19])(self->child);	/* Kill */
		self->child = 0;
		Obj_ChangeAnim(self, CLONE_PART(self->record, standAnim));
	}
	AI_NEXT_STEP(self, self->ai, 0);
}
