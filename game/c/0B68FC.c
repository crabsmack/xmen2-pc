/* BrainChild_GetSize - the size of a BRAINCHILD object (class slot +$06), $A6 bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long BrainChild_GetSize(void)
{
	return sizeof(BrainChild);
}
