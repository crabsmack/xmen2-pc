/* RollingDiscs_GetSize - the size of a ROLLINGDISCS object (class slot +$06), $90 bytes.
 * Never runs in the corpus. */
#include "xmen2.h"

long RollingDiscs_GetSize(void)
{
	return sizeof(RollingDiscs);
}
