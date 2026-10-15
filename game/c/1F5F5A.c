/* Cam_SetMaxXBlocks - set the camera's maximum x, and its target, so the screen's right edge
 * stops at a 32-px block column.
 *
 * col: the block column; g_CamMaxX = col * 32 - 320, and g_CamMaxXTarget the same. */
#include "xmen2.h"

void Cam_SetMaxXBlocks(short col)
{
	g_CamMaxX = (col << 5) - 0x140;
	g_CamMaxXTarget = g_CamMaxX;
}
