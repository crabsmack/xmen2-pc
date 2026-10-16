/* Tank_GetSize - the size of a TANK object (class slot +$06), $A8 bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long Tank_GetSize(void)
{
	return sizeof(Tank);
}
