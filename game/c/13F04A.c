/* Pop_GetSize - the size of a POP object (class slot +$06), $8E bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long Pop_GetSize(void)
{
	return sizeof(Pop);
}
