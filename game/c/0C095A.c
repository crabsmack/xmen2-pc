/* SwampFly_GetSize - the size of a SWAMPFLY object (class slot +$06), $B4 bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long SwampFly_GetSize(void)
{
	return sizeof(SwampFly);
}
