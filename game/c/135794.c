/* FlyBrood_StateHit - FLYBROOD's hurt state (set by FlyBrood_Init): burst at once
 * (ExplodeFx_BurstSmall).
 *
 * self: the object. */
#include "xmen2.h"

void FlyBrood_StateHit(FlyBrood *self)
{
	ExplodeFx_BurstSmall(self);
}
