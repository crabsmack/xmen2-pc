/* SmallBrood_ActApproach - SMALLBROOD AI-script action: walk to its distance from the nearest
 * player.
 *
 * self: the object.
 * Already within 4 pixels of keepDist (x distance): wait for the cooldown
 * (SmallBrood_StateWaitCooldown). Else face the player and walk (SmallBrood_StateApproach).
 * Returns flags & 8. */
#include "xmen2.h"

short SmallBrood_ActApproach(SmallBrood *self)
{
	XPlayer *p;
	short dx;

	p = Player_Nearest((Actor *)self);
	dx = (self->x - p->x) >> 8;
	dx = ABS(dx);
	if (dx >= self->keepDist - 4 && dx <= self->keepDist + 4) {
		SET_STATE(self, (StateFn)SmallBrood_StateWaitCooldown);
		return self->flags & 8;
	}
	if (p->x < self->x) {
		self->spr.flags |= 0x10;
		self->vx = -self->speed;
	} else {
		self->spr.flags &= ~0x10;
		self->vx = self->speed;
	}
	Obj_ChangeAnim(self, CODEPTR(Tbl_SmallBroodAnim));
	SET_STATE(self, SmallBrood_StateApproach);
	return self->flags & 8;
}
