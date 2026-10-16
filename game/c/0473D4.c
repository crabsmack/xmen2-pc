/* BaniMazaA2_UpdateDebris - one frame of the four debris groups (BaniMazaA2_UpdateDebrisGroup).
 * Never runs in the corpus (BaniMazaA2_FrameHook calls it while group 0 moves; nothing starts it). */
#include "xmen2.h"

#define g_A2Debris	(*(PTR(DebrisGroup) *)g_SceneVar3B14)	/* scene variable: the four debris groups */

void BaniMazaA2_UpdateDebris(void)
{
	register DebrisGroup *g = g_A2Debris;
	register short i;

	for (i = 0; i < 4; i++)
		BaniMazaA2_UpdateDebrisGroup(g++);
}
