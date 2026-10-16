/* ICandy_PoolFree - give an ICANDY pool slot back (clear poolFlags bit 15).
 *
 * slot: the object (ICandy_Destroy). */
#include "xmen2.h"

void ICandy_PoolFree(ICandy *slot)
{
	slot->poolFlags &= 0x7FFF;
}
