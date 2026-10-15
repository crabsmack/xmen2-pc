/* Cam_SetMinXBlocks - set the camera's minimum x, and its target, to a 32-px block column.
 *
 * col: the block column; g_CamMinX = col * 32, and g_CamMinXTarget the same (no easing). */
#include "xmen2.h"

void Cam_SetMinXBlocks(short col)
{
	g_CamMinX = col << 5;
	g_CamMinXTarget = g_CamMinX;
}
