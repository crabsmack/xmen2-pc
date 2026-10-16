/* BrainBabyBio_GetSize - the size of a BRAINBABYBIO object (class slot +$06), $C0 bytes.
 *
 * Never runs in the corpus. */
#include "xmen2.h"

long BrainBabyBio_GetSize(void)
{
	return sizeof(BrainBabyBio);
}
