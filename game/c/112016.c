/* Deathbird_Update - DEATHBIRD Update (class slot +$4A): the parent's (ACTOR) Update, then, unless
 * that stopped it (flags bit 4): while the second sprite is shown with a frame uploaded, the
 * second box is that frame's box2 (mirrored by the sprite's flips) around the position; and the
 * wing-flap sound $A9 replays at most every 13 frames, when the body shows frame 1.
 *
 * self: the boss.
 * (The do/while (0), probably a countdown macro's, makes MPW pop the Update call's argument
 * before the first branch, as the ROM does.) */
#include "xmen2.h"

#define g_DeathbirdWingTimer	(*(s16 *)g_SceneVar3B02)	/* scene variable: frames before the flap sound may play again */

void Deathbird_Update(Deathbird *self)
{
	SprFrame *f;
	short x, y;

	(*CLASS(Tbl_Deathbird_vtable)->parent->method[17])(self);	/* ACTOR Update */
	if (!(self->flags & 0x10)) {
		if (self->spr2Shown && self->spr2->uploaded != 0xFF) {
			f = SPR_FRAME(self->spr2->def, self->spr2->uploaded);
			x = self->x >> 8;
			y = self->y >> 8;
			if (self->spr2->flags & 0x10) {
				self->box2Left = x - f->box2Right;
				self->box2Right = x - f->box2Left;
			} else {
				self->box2Left = x + f->box2Left;
				self->box2Right = x + f->box2Right;
			}
			if (self->spr2->flags & 8) {
				self->box2Bottom = y - f->box2Top;
				self->box2Top = y - f->box2Bottom;
			} else {
				self->box2Bottom = y + f->box2Bottom;
				self->box2Top = y + f->box2Top;
			}
		}
		if (g_DeathbirdWingTimer) do {
			--g_DeathbirdWingTimer;
			if (g_DeathbirdWingTimer <= 0)
				g_DeathbirdWingTimer = 0;
		} while (0);
		if (!g_DeathbirdWingTimer && self->spr.frame == 1) {
			g_DeathbirdWingTimer = 13;
			Snd_Stop(0xA9);
			Snd_Play(0xA9);
		}
	}
}
