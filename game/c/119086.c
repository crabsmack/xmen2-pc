/* GaussBullet_UpdateAll - the gauss bullets' timer (every frame, GaussBullet_InitPool): update
 * each live bullet of g_GaussPool (GaussBullet_Update), 2 steps in two-player stepping, else 1.
 *
 * The end of the live records is re-read after each update; a bullet removed by its update has
 * the last one copied over it, which is then skipped this frame. */
#include "xmen2.h"

void GaussBullet_UpdateAll(void)
{
	register short steps = g_TwoPlayerStep ? 2 : 1;
	register GaussBullet *b;

	if (g_GaussPool->count) {
		for (b = g_GaussPool->rec; b < &g_GaussPool->rec[g_GaussPool->count]; b++)
			GaussBullet_Update(b, steps);
	}
}
