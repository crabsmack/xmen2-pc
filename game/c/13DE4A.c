/* ExplodeFx_BurstSmall - death effect: a small Burst (ExplodeFx_Burst kind 2,
 * Tbl_ExplodeFxSmall).
 *
 * obj: the dying object (killed). */
#include "xmen2.h"

void ExplodeFx_BurstSmall(void *obj)
{
	ExplodeFx_Burst(obj, 2);
}
