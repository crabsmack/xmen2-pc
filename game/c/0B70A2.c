/* ChildShip_GetSize - the size of a CHILDSHIP object (class slot +$06), $AE bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long ChildShip_GetSize(void)
{
	return sizeof(ChildShip);
}
