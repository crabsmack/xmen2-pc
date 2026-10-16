/* Mine_GetSize - the size of a MINE object (class slot +$06), $92 bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long Mine_GetSize(void)
{
	return sizeof(Mine);
}
