/* BabyBio_LevelEnd - BABYBIO's level end (class slot +$62; the parent's isn't called):
 * clear g_BabyBioUnused, a byte no other code reads or writes. */
#include "xmen2.h"

void BabyBio_LevelEnd(void)
{
	g_BabyBioUnused = 0;
}
