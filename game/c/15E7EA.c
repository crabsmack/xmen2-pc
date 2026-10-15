/* Actor_FreeSegCb - Seg_ForEachOfObj callback of Actor_Deactivate: free one floor segment of an
 * object.
 *
 * obj: the object (unused).  seg: its segment, freed (Seg_Free). */
#include "xmen2.h"

void Actor_FreeSegCb(Actor *obj, Seg *seg)
{
	Seg_Free(seg);
}
