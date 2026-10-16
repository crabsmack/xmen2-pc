/* MagneticGlob_Update - MAGNETICGLOB Update (class slot +$4A; its Timer runs it every frame).
 *
 * self: the globe.
 * Killed (slot +$52) once out of the active window. Else it moves (slot +$4E; 2 steps in the
 * two-player step mode), h-flipped as its heading says, plays its flight animation, and tests
 * the point 10 px behind it (against its motion): a body there in the other side's body list
 * (the enemies' for a player's globe, flags6 $10) is hit (MagneticGlob_HitCb) and the globe
 * explodes there (Explosion_Spawn, always in front: the priority test is an or); solid map there
 * ($7800 tile hooks, Tile_TestAt, the hooks run) makes it back out of the wall a quarter of its
 * velocity at a time and explode where it is free. */
#include "xmen2.h"

/* A position in pixels, passed to a short parameter (MPW pushes the long as it is). */
#ifdef XMEN2_MPW
#define PIXEL(v)	((v) >> 8)
#else
#define PIXEL(v)	((short)((v) >> 8))
#endif

void MagneticGlob_Update(register MagneticGlob *self)
{
	register short backX = self->vx > 0 ? -10 : self->vx < 0 ? 10 : 0;
	register short steps;
	register short backY = self->vy > 0 ? -10 : self->vy < 0 ? 10 : 0;
	register long x;
	register long y;
	register short px;
	register short py;

	if ((char)(*self->cls->method[15])(self)) {		/* +$42 OutsideActiveWindow */
		(*self->cls->method[19])(self);			/* +$52 Kill */
		return;
	}
	if (g_TwoPlayerStep)
		steps = 2;
	else
		steps = 1;
	(*self->cls->method[18])(self, steps);			/* +$4E Move */
	if (self->hflip)
		self->spr.flags |= 0x10;
	Obj_ChangeAnim(self, self->flyAnim);
	Anim_AdvanceByDistance(self, steps, 0, 0);
	x = self->x + (backX << 8);
	y = self->y + (backY << 8);
	px = (short)(x >> 8);
	py = (short)(y >> 8);
	if ((char)(self->flags6 & 0x10
		? ObjList_ForEachAtPoint(px, py, g_HitListEnemyBody, FN_DATA(MagneticGlob_HitCb), 0, (long)ADDR68(self))
		: ObjList_ForEachAtPoint(px, py, g_HitListPlayerBody, FN_DATA(MagneticGlob_HitCb), 0, (long)ADDR68(self)))) {
		Explosion_Spawn(x, y, self->spr.flags | 4 ? 1 : 0);
		(*self->cls->method[19])(self);			/* +$52 Kill */
		return;
	}
	if ((char)Tile_TestAt(self, px, py, !g_SolidMapIsBg, 0x7800, 1)) {
		register short stepX = self->vx >> 2;
		register short stepY = self->vy >> 2;

		for (;;) {
			x -= stepX;
			y -= stepY;
			if (!(char)Tile_TestAt(0, PIXEL(x), PIXEL(y), !g_SolidMapIsBg, 0x7800, 0))
				break;
		}
		Explosion_Spawn(x, y, 1);
		(*self->cls->method[19])(self);			/* +$52 Kill */
	}
}
