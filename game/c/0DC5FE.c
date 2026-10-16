/* Barrier_GetSize - the size of a BARRIER object (class slot +$06), $98 bytes. */
#include "xmen2.h"

long Barrier_GetSize(void)
{
	return sizeof(Barrier);
}
