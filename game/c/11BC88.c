/* BioFlame_Update - BIOFLAME Update (class slot +$4A; replaces the parent's): the three flame
 * sprites move 19 px further out each frame (forward, or back when h-flipped) and start again past
 * 96 px; the flame follows its owner (offset mirrored when h-flipped); a live player not flashing
 * from a hit, whose second box meets the 96 px of the flame at its height, is attacked (the
 * flame's Attack, class slot +$22: its hit type and attack).
 *
 * self: the flame.
 * A bug: the hit point's x is set for player 2 only (its x); for player 1 it is whatever the
 * caller left in the register (d4). The flame's span is taken before it follows its owner. */
#include "xmen2.h"

void BioFlame_Update(register BioFlame *self)
{
	short hx;
	register long px;
	register short py, left, right;

	if (self->spr.flags & 0x10) {
		self->flameX[0] -= 19;
		if (-self->flameX[0] > 0x60)
			self->flameX[0] = 0;
		self->flameX[1] -= 19;
		if (-self->flameX[1] > 0x60)
			self->flameX[1] = -10;
		self->flameX[2] -= 19;
		if (-self->flameX[2] > 0x60)
			self->flameX[2] = -20;
		px = self->x >> 8;
		left = (short)(px - 0x60);
		right = (short)px;
	} else {
		self->flameX[0] += 19;
		if (self->flameX[0] > 0x60)
			self->flameX[0] = 0;
		self->flameX[1] += 19;
		if (self->flameX[1] > 0x60)
			self->flameX[1] = 10;
		self->flameX[2] += 19;
		if (self->flameX[2] > 0x60)
			self->flameX[2] = 20;
		px = self->x >> 8;
		left = (short)px;
		right = (short)(px + 0x60);
	}
	if (self->owner) {
		if (!(self->spr.flags & 0x10))
			self->x = self->owner->x + (self->offX << 8);
		else
			self->x = self->owner->x - (self->offX << 8);
		self->y = self->owner->y + (self->offY << 8);
	}
#ifndef XMEN2_MPW
	hx = 0;		/* the 68000 passes the caller's d4 here (see above) */
#endif
	if (IS_ALIVE(&g_Player1) && !g_Player1.hitCooldown) {
		py = self->y >> 8;
		if (left <= g_Player1.box2Right && right >= g_Player1.box2Left && py >= g_Player1.box2Top
		    && py <= g_Player1.box2Bottom)
			(*self->cls->method[7])(self, &g_Player1, self->hitType, self->attack, hx, py);	/* +$22 Attack */
	}
	if (IS_ALIVE(&g_Player2) && !g_Player2.hitCooldown) {
		hx = g_Player2.x >> 8;
		py = self->y >> 8;
		if (left <= g_Player2.box2Right && right >= g_Player2.box2Left && py >= g_Player2.box2Top
		    && py <= g_Player2.box2Bottom)
			(*self->cls->method[7])(self, &g_Player2, self->hitType, self->attack, hx, py);	/* +$22 Attack */
	}
}
