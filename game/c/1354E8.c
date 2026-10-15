/* FlyBrood_ActSineFlyBy - FLYBROOD fly-by (after FlyBrood_StateWaitPlayerPassedSine): fly the way
 * it faces at its speed on a sine wave around its current y (FlyBrood_MoveSine from angle 0),
 * then fly off (FlyBrood_StateFlyOffKill).
 *
 * self: the object. Returns flags & 8.
 * The nearest player is looked up and not used (kept). Flying animation Tbl_Anim_13437C
 * (Tbl_Anim_134390 in the raw-art scene); flags6 bit $1000 set. */
#include "xmen2.h"

short FlyBrood_ActSineFlyBy(FlyBrood *self)
{
	XPlayer *p;
	short speed;
	short y;

	p = Player_Nearest((Actor *)self);
	speed = self->speed;
	y = self->py;
	if (self->spr.flags & 0x10)
		speed = -speed;
	self->vx = speed;
	self->anchorY = y;
	self->angle = 0;
	if (g_ScnUnused3B5C != g_SceneIndex)
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_13437C));
	else
		Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_134390));
	self->mover = FlyBrood_MoveSine;
	self->flags6 |= 0x1000;
	SET_STATE(self, (StateFn)FlyBrood_StateFlyOffKill);
	return self->flags & 8;
}
