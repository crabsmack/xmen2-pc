/* BioBullet_OutsideActiveWindow - BIOBULLET class slot +$42: is it outside the deactivation
 * window (g_DeactWin*, screen pixels around the camera)?
 *
 * self: the bullet. Returns 1 when it is (BioBullet_Update then kills it), else 0. */
#include "xmen2.h"

char BioBullet_OutsideActiveWindow(BioBullet *self)
{
	register short d = (self->x >> 8) - g_CameraX;

	if (d > g_DeactWinRight)
		return 1;
	if (d < g_DeactWinLeft)
		return 1;
	d = (self->y >> 8) - g_CameraY;
	if (d > g_DeactWinBottom)
		return 1;
	if (d < g_DeactWinTop)
		return 1;
	return 0;
}
