/* Brood_ActCharge - BROOD AI-script action (and its state after a hit, Brood_TakeDamage): face
 * the nearest player and walk at it (Brood_StateWalkToPlayer).
 *
 * self: the object.
 * Right after a hit (aiFlags bit 0, cleared here) it dodges instead: away from the player
 * (Brood_StateRetreatStart), or, when that end of its range is 10 pixels away or less, it is
 * cornered (Brood_StateCornered, 50 frames of hit cooldown). Returns flags & 8. */
#include "xmen2.h"

short Brood_ActCharge(Brood *self)
{
	XPlayer *p;

	p = Player_Nearest((Actor *)self);
	if (p->x < self->x)
		self->spr.flags |= 0x10;
	else
		self->spr.flags &= ~0x10;
	Obj_ChangeAnim(self, CODEPTR(Tbl_Anim_06D0D8));
	if (self->aiFlags & 1) {
		short x = self->x >> 8;
		short toMin = x - self->xMin;
		short toMax = x - self->xMax;

		toMin = ABS(toMin);
		toMax = ABS(toMax);
		self->aiFlags &= ~1;
		if (self->spr.flags & 0x10) {
			if (toMax > 10) {
				SET_STATE(self, Brood_StateRetreatStart);
				return self->flags & 8;
			}
			self->hitCooldown = 50;
			SET_STATE(self, Brood_StateCornered);
			return self->flags & 8;
		}
		if (toMin > 10) {
			SET_STATE(self, Brood_StateRetreatStart);
			return self->flags & 8;
		}
		self->hitCooldown = 50;
		SET_STATE(self, Brood_StateCornered);
		return self->flags & 8;
	}
	if (self->x < p->x)
		self->vx = self->speed;
	else
		self->vx = -self->speed;
	SET_STATE(self, Brood_StateWalkToPlayer);
	return self->flags & 8;
}
