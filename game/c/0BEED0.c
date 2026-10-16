/* SporeMine_RemoveSpore - take a spore out of the pool: one fewer in use, and the last one in
 * use is copied over it (the pool stays packed; the caller steps back to run the copy).
 *
 * s: the spore (a record of g_SporeMinePool). When s is the last one nothing is copied. */
#include "xmen2.h"

void SporeMine_RemoveSpore(Spore *s)
{
	g_SporeMinePoolCount--;
	if (g_SporeMinePoolCount > 0)
		Mem_Copy(s, &g_SporeMinePool[g_SporeMinePoolCount], sizeof(Spore));
}
