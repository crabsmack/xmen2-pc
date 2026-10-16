/* SpaceA1_FrameHook - the Space Elevator's scene hook (g_SceneFrameHook), once per frame: count the
 * frame and update the lift (g_Elevator's +$0E, Elevator_Update); in two-player step mode both
 * twice. */
#include "xmen2.h"

void SpaceA1_FrameHook(void)
{
	g_FrameCounter++;
	(*g_Elevator->cls->method[2])(g_Elevator);	/* Elevator_Update */
	if (g_TwoPlayerStep) {
		g_FrameCounter++;
		(*g_Elevator->cls->method[2])(g_Elevator);
	}
}
