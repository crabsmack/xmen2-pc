/* TechWallGun_Fire - TECHWALLGUN Fire (class slot +$6E): one bullet in a direction, from the
 * barrel: a free bullet record (TechWallGun_FindFreeBullet) starts as a muzzle flash (flags $40
 * | dir), the gun plays its shot animation and the shot sound ($F3).
 *
 * self: the gun.
 * dir:  1 up-right, 2 right, 3 down-right, 4 up-left, 5 left, 6 down-left (wall guns), 7
 *       down-left, 8 down, 9 down-right (ceiling guns): the barrel offset (pixels), the speed
 *       ($3A0 straight, $290 a side diagonally) and an acceleration of 14 / 10 the same way.
 * Another dir leaves the record set up and fires nothing. */
#include "xmen2.h"

void TechWallGun_Fire(TechWallGun *self, short dir)
{
	short ox, oy;
	WallGunBullet *b;
	short vx, vy, ax, ay;

	if ((b = TechWallGun_FindFreeBullet(self)) == 0)
		return;
	b->flags = 0x40;
	b->flags |= dir;
	Obj_SetAnim(self, self->fireAnim);
	switch (dir) {
	case 1:			/* TechWallGun_Fire_Case1 */
		ox = 0x1C;
		oy = -0x1B;
		vx = 0x290;
		vy = -0x290;
		ax = 10;
		ay = -10;
		break;
	case 2:
		ox = 0x22;
		oy = -7;
		vx = 0x3A0;
		vy = 0;
		ax = 14;
		ay = 0;
		break;
	case 3:
		ox = 0x1B;
		oy = 10;
		vx = 0x290;
		vy = 0x290;
		ax = 10;
		ay = 10;
		break;
	case 4:			/* TechWallGun_Fire_Case4 */
		ox = -0x2B;
		oy = -0x1B;
		vx = -0x290;
		vy = -0x290;
		ax = -10;
		ay = -10;
		break;
	case 5:			/* TechWallGun_Fire_Case5 */
		ox = -0x32;
		oy = -7;
		vx = -0x3A0;
		vy = 0;
		ax = -14;
		ay = 0;
		break;
	case 6:
		ox = -0x2B;
		oy = 10;
		vx = -0x290;
		vy = 0x290;
		ax = -10;
		ay = 10;
		break;
	case 7:
		ox = -0x1B;
		oy = 0x1B;
		vx = -0x290;
		vy = 0x290;
		ax = -10;
		ay = 10;
		break;
	case 8:			/* TechWallGun_Fire_Case8 */
		ox = -8;
		oy = 0x22;
		vx = 0;
		vy = 0x3A0;
		ax = 0;
		ay = 14;
		break;
	case 9:			/* TechWallGun_Fire_Case9 */
		ox = 10;
		oy = 0x1C;
		vx = 0x290;
		vy = 0x290;
		ax = 10;
		ay = 10;
		break;
	default:		/* TechWallGun_Fire_Default */
		b = 0;
		return;
	}
	/* TechWallGun_Fire_Spawn */
	b->frame = 0;
	b->x = self->x + ((long)ox << 8);
	b->y = self->y + ((long)oy << 8);
	b->vx = vx;
	b->vy = vy;
	b->ax = (s8)ax;
	b->ay = (s8)ay;
	Snd_Stop(0xF3);
	Snd_Play(0xF3);
	/* TechWallGun_Fire_Ret */
}
