/* FlyBrood_ActDiveAtPlayerLow - FLYBROOD dive (after FlyBrood_StateWaitPlayerPassedLow): as
 * FlyBrood_ActDiveAtPlayer, aiming 6 px below the nearest player's second box top.
 *
 * self: the object. Returns flags & 8. */
#include "xmen2.h"

short FlyBrood_ActDiveAtPlayerLow(FlyBrood *self)
{
	XPlayer *p;
	long dx;
	long ty;
	long dy;
	short speed;
	long t;
	short vy;

	p = Player_Nearest((Actor *)self);
	dx = p->x - self->x;
	ty = (p->box2Top + 6) << 8;
	dy = ty - self->y;
	speed = self->speed;
	t = ABS(dx) / (speed ? speed : 0x200);
	vy = (short)(dy / (t ? t : 1));
	if (self->spr.flags & 0x10)
		speed = -speed;
	self->vx = speed;
	self->vy = vy;
	if (g_ScnUnused3B5C != g_SceneIndex)
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_13437C));
	else
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_134390));
	self->mover = FlyBrood_MoveBallistic;
	self->anchorY = (short)(ty >> 8);
	self->flags6 |= 0x1000;
	SET_STATE(self, (StateFn)FlyBrood_StateFlyOffKill);
	return self->flags & 8;
}
