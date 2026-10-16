/* TrapDoor_GetSize - the size of a TRAPDOOR object (class slot +$06), $A0 bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long TrapDoor_GetSize(void)
{
	return sizeof(TrapDoor);
}
