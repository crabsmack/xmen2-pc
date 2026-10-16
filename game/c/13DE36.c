/* ExplodeFx_BurstMedium - death effect: a medium Burst (ExplodeFx_Burst kind 1,
 * Tbl_ExplodeFxMedium).
 *
 * obj: the dying object (killed). */
#include "xmen2.h"

void ExplodeFx_BurstMedium(void *obj)
{
	ExplodeFx_Burst(obj, 1);
}
