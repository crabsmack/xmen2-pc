/* BrainRocket_OutsideActiveWindow - BRAINROCKET class slot +$42: is it outside the deactivation
 * window (g_DeactWin*, screen pixels around the camera) widened by its margins (marginX / Y)?
 *
 * self: the rocket. Returns 1 when it is, else 0. */
#include "xmen2.h"

char BrainRocket_OutsideActiveWindow(BrainRocket *self)
{
	register short sx = (self->x >> 8) - g_CameraX;
	register short sy = (self->y >> 8) - g_CameraY;

	if (sx > g_DeactWinRight + self->marginX)
		return 1;
	if (sx < g_DeactWinLeft - self->marginX)
		return 1;
	if (sy > g_DeactWinBottom + self->marginY)
		return 1;
	if (sy < g_DeactWinTop - self->marginY)
		return 1;
	return 0;
}
