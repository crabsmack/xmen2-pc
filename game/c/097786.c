/* XClone_ActAnim3C - XCLONE script action: play the character's down-attack animation to its end
 * ($FF: the animation's end goes on with the script); a character without one takes the next step
 * on the next update.
 *
 * self: the clone (a step of its AI script). */
#include "xmen2.h"

void XClone_ActAnim3C(XClone *self)
{
	if (self->charRec->downAttackAnim) {
		Obj_SetAnim(self, CLONE_PART(self->charRec, downAttackAnim));
		AI_NEXT_STEP(self, self->ai, 0xFF);
	} else {
		AI_NEXT_STEP(self, self->ai, 0);
	}
}
