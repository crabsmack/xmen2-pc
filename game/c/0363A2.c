/* MagnetoBoss_Update - MAGNETOBOSS Update (class slot +$4A): the parent's (ACTOR) Update, then,
 * unless flags bit $10: the shot cooldown counts down (twice in two-player steps), every 60
 * frames the nearest live player becomes the target, it faces the target (bossFlags bit 4,
 * not while hit), and the idle animation (Tbl_MagnetoBossAnim2) restarts when none plays.
 *
 * self: the boss. */
#include "xmen2.h"

void MagnetoBoss_Update(register MagnetoBoss *self)
{
	register short n = g_TwoPlayerStep ? 2 : 1;
	register Actor *t;

	(*CLASS(Tbl_MagnetoBoss_vtable)->parent->method[17])(self);	/* ACTOR Update */
	if (self->flags & 0x10)
		return;
	do {
		if (self->shotCooldown) {
			--self->shotCooldown;
			if (self->shotCooldown < 0)
				self->shotCooldown = 0;
		}
	} while (--n);
	if (!self->countdown) {
		self->target = Player_NearestLive(self);
		self->countdown = 60;
	}
	if ((self->bossFlags & 0x10) && (t = self->target) && !self->hitCooldown) {
		if (t->x < self->x)
			self->spr.flags |= 0x10;
		else
			self->spr.flags &= ~0x10;
	}
	if (!self->anim)
		Obj_ChangeAnim(self, CODEPTR(Tbl_MagnetoBossAnim2));
}
