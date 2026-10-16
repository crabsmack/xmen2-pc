/* Spark_StateHit - SPARK hurt state (hurtState): blow up, an Explosion (priority 1) at its
 * position, and Kill.
 *
 * self: the object. Never runs in the corpus. */
#include "xmen2.h"

void Spark_StateHit(register Spark *self)
{
	Explosion_Spawn(self->x, self->y, 1);
	(*self->cls->method[19])(self);			/* +$52 Kill */
}
