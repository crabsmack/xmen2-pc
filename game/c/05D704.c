/* Tuskette_StateHitTarget - TUSKETTE of a pair that reached its partner: a small burst
 * (ExplodeFx_BurstSmall).
 *
 * self: the object. Never runs in the corpus. Keeps this state: it bursts again whenever the
 * state runs. */
#include "xmen2.h"

void Tuskette_StateHitTarget(Tuskette *self)
{
	ExplodeFx_BurstSmall((Actor *)self);
}
