/* TechWallGun_AimWall - can a wall gun aim at the nearest player (Player_Nearest)? The player
 * must be less than range px away in x and in front of it (the side it faces: h-flip = left).
 *
 * self:   the gun.
 * *angle: then the direction to the player's chest (30 px above its position), mirrored to the
 *         front: degrees (TechWallGun_AimAngle), negative = up.
 * Returns 1 when it can aim, else 0 (*angle untouched). */
#include "xmen2.h"

char TechWallGun_AimWall(TechWallGun *self, s16 *angle)
{
	Actor *p = (Actor *)Player_Nearest((Actor *)self);
	short dx = (p->x - self->x) >> 8;
	short dy, adx;

	dy = ((p->y - self->y) >> 8) - 30;
	adx = ABS(dx);
	if (adx >= self->range || (self->spr.flags & 0x10 ? dx > 0 : dx < 0))
		return 0;
	*angle = (TechWallGun_AimAngle(adx, dy) + 2) >> 2;
	return 1;
}
