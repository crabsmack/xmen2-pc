/* FlyBrood_ActDiveAtPlayer - FLYBROOD dive (after FlyBrood_StateWaitPlayerPassed): fly the way it
 * faces at its speed, with the y speed that reaches 20 px below the nearest player's second box
 * top when it has covered the x distance; stop there vertically (FlyBrood_MoveBallistic), then
 * fly off (FlyBrood_StateFlyOffKill).
 *
 * self: the object. Returns flags & 8.
 * Frames to the player = |x distance| / speed ($200 when 0), at least 1. Diving animation
 * Tbl_Anim_13437C (Tbl_Anim_134390 in the raw-art scene); flags6 bit $1000 set. */
#include "xmen2.h"

short FlyBrood_ActDiveAtPlayer(FlyBrood *self)
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
	ty = (p->box2Top + 0x14) << 8;
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
