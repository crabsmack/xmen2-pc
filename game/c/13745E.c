/* HornetBullet_Update - HORNETBULLET Update (class slot +$4A; replaces the parent's): killed
 * outside the active window, or when it leaves the FG map or reaches a solid cell (block flag
 * $4000); else it moves by its velocity (twice in two-player step mode, no acceleration) and,
 * unless the scene is ending (g_SceneExitTimer), hits each live player not flashing from a hit
 * (hitCooldown 0) whose second box holds its position, through its Attack (class slot +$22) with
 * its hitType and attack.
 *
 * self: the bullet. Never runs in the corpus. */
#include "xmen2.h"

void HornetBullet_Update(register HornetBullet *self)
{
	register long x, y;
	register long i;
	register XPlayer *pl;

	if ((char)(*self->cls->method[15])(self)) {		/* OutsideActiveWindow */
		(*self->cls->method[19])(self);			/* Kill */
		return;
	}
	self->x += self->vx;
	self->y += self->vy;
	if (g_TwoPlayerStep) {
		self->x += self->vx;
		self->y += self->vy;
	}
	x = self->x >> 8;
	y = self->y >> 8;
	if (x >> 5 >= 0 && x >> 5 < g_FgMapW && y >> 5 >= 0 && y >> 5 < g_FgMapH
	    ? (g_FgBlockMap[g_FgRowTable[y >> 5] + (x >> 5)] & 0x4000 ? 1 : 0) : 1) {
		(*self->cls->method[19])(self);			/* Kill */
		return;
	}
	if (!g_SceneExitTimer) {
		for (i = 0; i < 2; ++i) {
			if (IS_ALIVE(i == 0 ? (pl = &g_Player1) : (pl = &g_Player2)) && !pl->hitCooldown
			    && x >= pl->box2Left && x <= pl->box2Right
			    && y >= pl->box2Top && y <= pl->box2Bottom)
				(*self->cls->method[7])(self, pl, self->hitType, self->attack, x, y);	/* Attack */
		}
	}
}
