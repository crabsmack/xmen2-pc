/* XClone_ActBecomeBalls - XCLONE script action: turn into a BIOBALLS object (reused or new; sound
 * $F2): the clone is hidden and unhittable, the balls (followed by the arena camera) take its
 * place facing its way, in the clone record's ball path formation, with pod script
 * XClone_PodScript and animation Tbl_XCloneAnim7 (BioBalls_ActClearAnim at its end); then wait
 * for them (XClone_StateWaitChild).
 *
 * self: the clone (a step of its AI script). Returns flags & 8. */
#include "xmen2.h"

#define g_CloneArenaFocus	g_SceneVar3B30	/* scene variable: object the arena camera follows */

short XClone_ActBecomeBalls(XClone *self)
{
	BioBalls *balls;

	self->anim = 0;
	(*self->cls->method[20])(self, 0xFF);		/* SetFrame: none */
	if (self->child)
		balls = self->child;
	else
		balls = (BioBalls *)(*CLASS(Tbl_BioBalls_vtable)->method[2])(0,	/* New */
			CLASS(Tbl_BioBalls_vtable), self->x, self->y, 0);
	if (balls) {
		g_CloneArenaFocus = (Actor *)balls;
		Snd_Stop(0xF2);
		Snd_Play(0xF2);
		self->flags6 |= 2;
		List_Unlink(self->bodyNode, &g_HitListEnemyBody);
		self->bodyNode = 0;
		if (!(self->spr.flags & 0x10))
			balls->spr.flags &= ~0x10;
		else
			balls->spr.flags |= 0x10;
		BioBalls_SetFormation(balls, CLONE_PART(self->record, ballPath));
		AiScript_Set(&balls->ai, CODEPTR(Tbl_XClone_PodScript));
		balls->x = self->x;
		balls->y = self->y;
		Obj_SetAnim(balls, CODEPTR(Tbl_XCloneAnim7));
		balls->done = 0;
		balls->state = (StateFn)BioBalls_ActClearAnim;
		balls->timer = 0xFF;
		balls->ballFlags |= 0x100;
		self->child = balls;
	}
	SET_STATE(self, XClone_StateWaitChild);
	return self->flags & 8;
}
