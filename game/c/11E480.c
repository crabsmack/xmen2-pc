/* BioBullet_Update - BIOBULLET Update (class slot +$4A; replaces the parent's): killed outside
 * the active window (its slot +$42); else it moves by its velocity and acceleration (twice in
 * two-player step mode), then explodes and is killed when it leaves the FG map or reaches a solid
 * cell (block flag $4000), or when it hits a player's body (BioBullet_HitActor).
 *
 * self: the bullet.
 * The explosion takes the bullet's sprite priority (flags bit 2). */
#include "xmen2.h"

void BioBullet_Update(BioBullet *self)
{
	register short x, y;

	if ((char)(*self->cls->method[15])(self)) {		/* +$42 OutsideActiveWindow */
		(*self->cls->method[19])(self);			/* +$52 Kill */
		return;
	}
	self->x += self->vx;
	self->y += self->vy;
	self->vx += self->ax;
	self->vy += self->ay;
	if (g_TwoPlayerStep) {
		self->x += self->vx;
		self->y += self->vy;
		self->vx += self->ax;
		self->vy += self->ay;
	}
	x = self->x >> 8;
	y = self->y >> 8;
	if ((x >> 5 >= 0 && x >> 5 < g_FgMapW && y >> 5 >= 0 && y >> 5 < g_FgMapH)
	    ? (g_FgBlockMap[g_FgRowTable[y >> 5] + (x >> 5)] & 0x4000 ? 1 : 0) : 1) {
		Explosion_Spawn(self->x, self->y, self->spr.flags & 4 ? 1 : 0);
		(*self->cls->method[19])(self);			/* +$52 Kill */
	} else if (ObjList_ForEachAtPoint(x, y, g_HitListPlayerBody, FN_DATA(BioBullet_HitActor), 0, 0)) {
		Explosion_Spawn(self->x, self->y, self->spr.flags & 4 ? 1 : 0);
		(*self->cls->method[19])(self);			/* +$52 Kill */
	}
}
