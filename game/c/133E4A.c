/* BigAcBullet_Attack - BIGACBULLET Attack (class slot +$22): hit the target (its TakeDamage) as hit
 * type 8 whatever type it was given, no hit cooldown, then the bullet is killed (slot +$52).
 *
 * self: the bullet.  target: what it touched.  type: the hit type (ignored).  damage: the hit's
 * damage.  hx, hy: the hit point. */
#include "xmen2.h"

void BigAcBullet_Attack(register BigAcBullet *self, register Actor *target, char type, char damage,
			short hx, short hy)
{
	(*target->cls->method[8])(target, self, 8, damage, hx, hy);	/* +$26 TakeDamage */
	(*self->cls->method[19])(self);					/* +$52 Kill */
}
