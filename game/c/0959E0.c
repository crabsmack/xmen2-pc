/* XClone_ActUnhittableScript - XCLONE: become unhittable (off the enemy body hit list, flags6
 * bits 2 and $1000) and run the entrance script XClone_Script1 from its first step, now.
 *
 * self: the clone (a script action; BroodClaw_StateDropClone also calls it on a new clone). */
#include "xmen2.h"

void XClone_ActUnhittableScript(XClone *self)
{
	if (!(self->flags6 & 2)) {
		self->flags6 |= 2;
		List_Unlink(self->bodyNode, &g_HitListEnemyBody);
		self->bodyNode = 0;
	}
	self->flags6 |= 0x1000;
	AiScript_Set(&self->ai, CODEPTR(Tbl_XClone_Script1));
	AI_FIRST_STEP(self, self->ai, 0);
}
