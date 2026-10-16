/* XClone_ActOpenBioBalls - XCLONE script action: hide the clone and open a BIOBALLS effect on it
 * (reused or new; sound $F2): facing the clone's way, the formation Tbl_XCloneOpenFormation 28 px
 * up, pod script XClone_PodScript, animation Tbl_XCloneAnim7 (BioBalls_ActClearAnim at its end),
 * orbit easing flag; then wait for it (XClone_StateWaitChild).
 *
 * self: the clone (a step of its AI script). Returns flags & 8. */
#include "xmen2.h"

short XClone_ActOpenBioBalls(XClone *self)
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
		Snd_Stop(0xF2);
		Snd_Play(0xF2);
		if (!(self->spr.flags & 0x10))
			balls->spr.flags &= ~0x10;
		else
			balls->spr.flags |= 0x10;
		BioBalls_SetFormation(balls, CODEPTR(Tbl_XCloneOpenFormation));
		AiScript_Set(&balls->ai, CODEPTR(Tbl_XClone_PodScript));
		balls->x = self->x;
		balls->y = self->y;
		balls->pathDx = 0;
		balls->pathDy = -0x1C;
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
