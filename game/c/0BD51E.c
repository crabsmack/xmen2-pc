/* Barbarian_GetSize - the size of a BARBARIAN object (class slot +$06), $C2 bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long Barbarian_GetSize(void)
{
	return sizeof(Barbarian);
}
