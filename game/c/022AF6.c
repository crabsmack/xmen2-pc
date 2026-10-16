/* AvalonA2_PlacePlayers - put the players at the Falling Room's start (Players_PlaceAtStart, all
 * arguments 0); the retracting floors drop $300 deep (g_RetractFloorDepth) and the players fall
 * slowly at that speed (g_FloatFallSpeed). */
#include "xmen2.h"

void AvalonA2_PlacePlayers(void)
{
	Players_PlaceAtStart(0, 0, 0, 0);
	g_RetractFloorDepth = 0x300;
	g_FloatFallSpeed = g_RetractFloorDepth;
}
