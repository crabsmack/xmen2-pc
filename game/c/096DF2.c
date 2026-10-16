/* XClone_StateChildShoot - XCLONE state: the balls form fires a BIOBULLET at the nearest player
 * (sound $F4) every 5 frames until shotsLeft runs out, then the next script step on the next
 * update.
 *
 * self: the clone (state set by XClone_ActChildShoot3; never runs in the corpus).
 * The bullet's speed is the distance to the player's body-box centre (8 fraction bits) shifted
 * down by the highest of bits 7..1 set in (|dx| + |dy|) >> 10 (0 when none is).
 * Returns flags & 8 between shots; nothing in particular when the script goes on. */
#include "xmen2.h"

short XClone_StateChildShoot(XClone *self)
{
	XPlayer *p;
	Actor *bullet;
	BioBalls *balls;
	long tx, ty, dx, dy;
	short dist, bit, vx, vy;

	p = Player_Nearest((Actor *)self);
	balls = self->child;
	if (bullet = (Actor *)(*CLASS(Tbl_BioBullet_vtable)->method[2])(0,	/* New */
			CLASS(Tbl_BioBullet_vtable), balls->x, balls->y, 0)) {
		tx = p->x;
		ty = ((p->boxTop + p->boxBottom) >> 1) << 8;
		dx = tx - balls->x;
		dy = ty - balls->y;
		dist = (ABS(dx) + ABS(dy)) >> 10;
		for (bit = 7; bit > 0; bit--)
			if (1L << bit & dist)
				break;
		vx = dx >> bit;
		vy = dy >> bit;
		BioBullet_SetVelocity(bullet, vx, vy, 1);
	}
	Snd_Stop(0xF4);
	Snd_Play(0xF4);
	--self->shotsLeft;
	if (self->shotsLeft > 0) {
		SET_STATE_WAIT(self, XClone_StateChildShoot, 5);
		return self->flags & 8;
	}
	AI_NEXT_STEP(self, self->ai, 0);
	NO_RETURN_VALUE
}
