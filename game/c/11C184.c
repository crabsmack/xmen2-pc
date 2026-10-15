/* BabyBio_LevelStart - per-level start of BABYBIO (class slot +$62; the parent's isn't called):
 * clear g_BabyBioUnused, a byte no other code reads or writes. */
#include "xmen2.h"

void BabyBio_LevelStart(void)
{
	g_BabyBioUnused = 0;
}
