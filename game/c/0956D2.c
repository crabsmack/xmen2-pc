/* XClone_Explode - XCLONE's death (class slot +$7E): become unhittable, drop the hurt flags and
 * run the death script (XClone_Script2) from its first step, now.
 *
 * self: the clone. */
#include "xmen2.h"

void XClone_Explode(XClone *self)
{
	if (!(self->flags6 & 2)) {
		self->flags6 |= 2;
		List_Unlink(self->bodyNode, &g_HitListEnemyBody);
		self->bodyNode = 0;
	}
	self->flags &= ~0x200;
	self->flags &= ~4;
	self->flags6 &= ~0x4000;
	AiScript_Set(&self->ai, CODEPTR(Tbl_XClone_Script2Len));
	AI_FIRST_STEP(self, self->ai, 0);
}
