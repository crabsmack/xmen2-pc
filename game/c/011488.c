/* BigAcolyte_StateShoot - BIGACOLYTE state: fire a BIGACBULLET from its hand (50 px ahead, 46 px
 * up, the way it faces: direction 0 or 180), back to the idle animation (Tbl_BigAcolyteAnim).
 * Not yet at its target height: shoot again in 3 frames (BigAcolyte_StateShootAnim). There: at
 * the low height (level 1) check where the players are (BigAcolyte_StateCheckPlayersLow),
 * otherwise the next step of its AI script.
 *
 * self: the boss.
 * Returns self->flags & 8, nothing when the script goes on. */
#include "xmen2.h"

short BigAcolyte_StateShoot(register BigAcolyte *self)
{
	register short dx, dy;
	register short dir;
	register Actor *bullet;

	dx = (self->spr.flags & 0x10) ? -50 : 50;
	dy = -46;
	dir = (unsigned char)(!(self->spr.flags & 0x10) ? 0 : 0xB4);
	bullet = (Actor *)(*CLASS(Tbl_BigAcBullet_vtable)->method[2])(0,	/* New */
		CLASS(Tbl_BigAcBullet_vtable), self->x, self->y, 0);
	if (bullet)
		BigAcBullet_Attach(bullet, self, dx, dy, dir);
	Obj_SetAnim(self, CODEPTR(Tbl_BigAcolyteAnim));
	if (self->arrivedY) {
		if (self->level == 1) {
			SET_STATE(self, BigAcolyte_StateCheckPlayersLow);
			return self->flags & 8;
		}
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		SET_STATE_WAIT(self, BigAcolyte_StateShootAnim, 3);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
