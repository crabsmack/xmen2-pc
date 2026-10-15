/* Brood_ActApproach - BROOD AI-script action: walk to its distance from the nearest player.
 *
 * self: the object.
 * Already within 4 pixels of keepDist (x distance): next step. Else, when the cooldown has
 * run out: reload it, next step. Else face the player and walk (Brood_StateApproach), returning
 * flags & 8 (nothing when a script step follows). */
#include "xmen2.h"

short Brood_ActApproach(Brood *self)
{
	XPlayer *p;
	short dx;

	p = Player_Nearest((Actor *)self);
	dx = (self->x - p->x) >> 8;
	dx = ABS(dx);
	if (dx >= self->keepDist - 4 && dx <= self->keepDist + 4) {
		AI_NEXT_STEP(self, self->ai, 0);
		RETURN_NO_VALUE;
	}
	if (self->cooldown == 0) {
		self->cooldown = self->cooldownTime;
		AI_NEXT_STEP(self, self->ai, 0);
		RETURN_NO_VALUE;
	}
	if (p->x < self->x) {
		self->spr.flags |= 0x10;
		self->vx = -self->speed;
	} else {
		self->spr.flags &= ~0x10;
		self->vx = self->speed;
	}
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0F0));
	SET_STATE(self, Brood_StateApproach);
	return self->flags & 8;
}
