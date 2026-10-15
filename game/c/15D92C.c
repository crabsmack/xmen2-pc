/* Actor_GetSize - the size of an ACTOR object (class slot +$06), $8C bytes.
 *
 * The spawner (Synchronizer_New) calls it on a class to size the allocation. Never runs in
 * the corpus. */
#include "xmen2.h"

long Actor_GetSize(void)
{
	return sizeof(Actor);
}
