/* BigBio_SpitFree - remove a spit projectile from the pool: the last one moves into its record.
 *
 * spit: a record in use in g_BigBioSpitPool (BigBio_SpitUpdateAll steps back over it). */
#include "xmen2.h"

void BigBio_SpitFree(BigBioSpit *spit)
{
	if (g_BigBioSpitCount)
		Mem_Copy(spit, &g_BigBioSpitPool[--g_BigBioSpitCount], sizeof(BigBioSpit));
}
