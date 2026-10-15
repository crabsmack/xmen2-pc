/* XClone_ActLeaveBalls - XCLONE script action: come back out of the balls form: remove the
 * BIOBALLS object (sound $F2), stand, become the object the arena camera follows, hittable again,
 * and switch to the clone record's main script; then go on with the script now.
 *
 * self: the clone (a step of its AI script).
 * The "go on" steps the new main script to its second step (AiScript_Set starts it at 0).
 * Without a BIOBALLS object only the step is taken. */
#include "xmen2.h"

#define g_CloneArenaFocus	g_SceneVar3B30	/* scene variable: object the arena camera follows */

void XClone_ActLeaveBalls(XClone *self)
{
	if (self->child) {
		Snd_Stop(0xF2);
		Snd_Play(0xF2);
		(*self->child->cls->method[19])(self->child);	/* Kill */
		self->child = 0;
		Obj_ChangeAnim(self, CLONE_PART(self->record, standAnim));
		g_CloneArenaFocus = (Actor *)self;
		self->flags6 &= ~2;
		self->bodyNode = HitList_Add(self, &g_HitListEnemyBody);
		AiScript_Set(&self->ai, CLONE_PART(self->record, mainScript));
	}
	AI_NEXT_STEP(self, self->ai, 0);
}
