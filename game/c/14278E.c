/* TechWallGun_AimCeiling - can a ceiling gun aim at the nearest player (Player_Nearest)? The
 * player must be less than range px away in x and its chest (30 px above its position) not
 * above the gun.
 *
 * self:   the gun.
 * *angle: then the direction to the player's chest in degrees (TechWallGun_AimAngle): 0 right,
 *         90 straight down, 180 left.
 * Returns 1 when it can aim, else 0 (*angle untouched). */
#include "xmen2.h"

char TechWallGun_AimCeiling(TechWallGun *self, s16 *angle)
{
	Actor *p = (Actor *)Player_Nearest((Actor *)self);
	short dx = (p->x - self->x) >> 8;
	short dy, adx;

	dy = ((p->y - self->y) >> 8) - 30;
	adx = ABS(dx);
	if (adx >= self->range || dy < 0)
		return 0;
	*angle = (TechWallGun_AimAngle(dx, dy) + 2) >> 2;
	return 1;
}
