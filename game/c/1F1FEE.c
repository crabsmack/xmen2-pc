/* XPlayer_Attack - XPLAYER class slot +$22: hit a target with the current attack and show the hit
 * spark, sized by the attack's strength.
 *
 * self:   the player (attack: its strength; 0 = no attack, nothing happens).
 * target: the object hit.
 * type:   hit type; 9 becomes 10 when the target is to the right.
 * damage: passed to the target's TakeDamage.
 * sx, sy: where the spark shows (pixels); sx -1: no spark.
 * Clears stateTimer and actionTimer, 15 frames of hit cooldown. The spark faces like the player and
 * lasts 8/10/12/14 frames, frame 1/2/3/3, for strength 1/2/4/8 (others as 1). */
#include "xmen2.h"

void XPlayer_Attack(XPlayer *self, Actor *target, char type, short damage, short sx, short sy)
{
	if (self->attack) {
		if (type == 9 && target && self->x < target->x)
			type = 10;
		self->stateTimer = 0;
		self->actionTimer = 0;
		(*target->cls->method[8])(target, self, type, damage, sx, sy);	/* TakeDamage */
		self->hitCooldown = 15;
		if (sx != -1) {
			if (self->spr.flags & 4)
				g_HitSparkSpr.flags |= 4;
			else
				g_HitSparkSpr.flags &= ~4;
			self->sparkX = sx;
			self->sparkY = sy;
			switch (self->attack) {
			default:
			case 1:
				self->sparkTimer = 8;
				g_HitSparkSpr.frame = 1;
				break;
			case 2:
				self->sparkTimer = 10;
				g_HitSparkSpr.frame = 2;
				break;
			case 4:
				self->sparkTimer = 12;
				g_HitSparkSpr.frame = 3;
				break;
			case 8:
				self->sparkTimer = 14;
				g_HitSparkSpr.frame = 3;
				break;
			}
		}
	}
}
