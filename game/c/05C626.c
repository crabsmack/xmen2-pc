/* Tusk_AtkThrowBoulder - TUSK attack step: make a boulder (Boulder_Spawn kind 1: held) at its
 * position, aim at the target player (aimColumn = (target x - home x + 16) / 32), play the
 * throw animation with the boulder attached as its child, and throw it when the animation
 * ends (Tusk_StateThrow, wait $FF). No boulder: choose again (Tusk_StateChooseAttack).
 *
 * self: TUSK.
 * Returns 0.
 * The empty if (0) after the state change is a block the original compiled out: its end is a
 * label, and that is where MPW C pops the calls' arguments (between the two stores). */
#include "xmen2.h"

short Tusk_AtkThrowBoulder(Tusk *self)
{
	Boulder *boulder;
	register XPlayer *p;

	if (boulder = Boulder_Spawn(self->x, self->y, 1)) {
		p = self->target;
		self->aimColumn = ((p->x >> 8) - self->px + 16) >> 5;
		Obj_SetAnim(self, CODEPTR(Tbl_TuskAnim2));
		Obj_AttachChild(self, boulder);
		self->state = Tusk_StateThrow;
		if (0) {				/* compiled out (see the header) */
		}
		self->timer = 0xFF;
		return 0;
	} else {
		self->state = Tusk_StateChooseAttack;
		return 0;
	}
}
