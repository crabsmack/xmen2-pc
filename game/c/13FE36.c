/* Spark_Attack - SPARK Attack (class slot +$22; the parent's isn't called): hit the target
 * (its TakeDamage, the spark as attacker), then blow up: an Explosion (priority 1) at the centre
 * of its body box, and Kill.
 *
 * self: the spark.  target: the object hit.  type, damage, x, y: the hit (passed on).
 * Never runs in the corpus. */
#include "xmen2.h"

void Spark_Attack(register Spark *self, register Actor *target, char type, char damage, short x, short y)
{
	(*target->cls->method[8])(target, self, type, damage, x, y);	/* +$26 TakeDamage */
	Explosion_Spawn((self->boxRight + self->boxLeft) << 7, (self->boxTop + self->boxBottom) << 7, 1);
	(*self->cls->method[19])(self);			/* +$52 Kill */
}
