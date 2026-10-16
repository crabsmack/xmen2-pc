/* AvalonA2_RaiseCameraFloor - the Falling Room's camera stops $C0 px higher: the scroll's bottom
 * limit g_CamMaxY and its target go up by $C0. */
#include "xmen2.h"

void AvalonA2_RaiseCameraFloor(void)
{
	g_CamMaxY -= 0xC0;
	g_CamMaxYTarget = g_CamMaxY;
}
