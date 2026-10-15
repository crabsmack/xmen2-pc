/* XPlayer_TetherToPartner - two players: keep this one within a screen of its partner (when the
 * partner is present and alive), and start the fall-and-rejoin when it is left far below.
 *
 * self: the player.
 * x is kept within $11A (282) px of the partner's (XPlayer_TetherClamp records the blocked side:
 * playerFlags $4000 left, $8000 right); y no more than $E0 px above it ($10000; vy stopped)
 * unless the partner itself fell ($20000), nor $E0 px below it ($20000), unless the partner is
 * in the air and not climbing. In the air and not climbing, far below: playerFlags $20000 and,
 * the first frame (not set before) and still alive, normal gravity, 60 frames unhittable,
 * flags6 $80, the camera's y locked and the state XPlayer_StartFallOffScreen: returns 1.
 * Else returns 0. The flags $3C000 are recomputed each call. */
#include "xmen2.h"

char XPlayer_TetherToPartner(XPlayer *self)
{
	XPlayer *partner;
	long px, py;
	long x, y;
	long d;
	long was;

	partner = self == &g_Player1 ? &g_Player2 : &g_Player1;
	x = self->x;
	y = self->y;
	px = partner->x;
	py = partner->y;
	was = 0;
	if (!IS_ALIVE(partner))
		return 0;
	was = self->playerFlags & 0x3C000;
	self->playerFlags = self->playerFlags & ~0x3C000 | 0;
	d = x - px;
	if (d < 0) {
		d += 0x11A00;
		if (d < 0)
			XPlayer_TetherClamp(self, &self->x, x - d, 0x4000);
	} else {
		d -= 0x11A00;
		if (d > 0)
			XPlayer_TetherClamp(self, &self->x, x - d, 0x8000);
	}
	d = y - py;
	if (d < 0) {
		d += 0xE000;
		if (d < 0 && !(partner->playerFlags & 0x20000)) {
			self->vy = 0;
			XPlayer_TetherClamp(self, &self->y, y - d, 0x10000);
		}
	} else {
		d -= 0xE000;
		if (d > 0 && (!(partner->flags & 0x1000) || (char)Player_ClimbState(partner))) {
			if (self->spr.unk_0C != 0xFF)
				Obj_Detach(self);
			if (self->flags & 0x1000 && !(char)Player_ClimbState(self)) {
				self->playerFlags |= 0x20000;
				if (!(was & 0x20000) && self->health > 0) {
					self->ay = self->charRec->gravity;
					self->timer = 0;
					self->hitCooldown = 60;
					self->flags6 |= 0x80;
					g_Camera->camFlags |= 2;
					self->state = (StateFn)XPlayer_StartFallOffScreen;
					return 1;
				}
			} else
				XPlayer_TetherClamp(self, &self->y, y - d, 0x20000);
		}
	}
	return 0;
}
