/* SpikeBall_GetSize - the size of a SPIKEBALL object (class slot +$06), $90 bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long SpikeBall_GetSize(void)
{
	return sizeof(SpikeBall);
}
