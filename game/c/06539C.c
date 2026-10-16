/* PlayerSelect_CheckDone - once every player taking part has confirmed a character (only player 1,
 * only player 2, or both), the player select ends in 2 frames (g_SceneExitTimer), unless its end
 * is already counting down. */
#include "xmen2.h"

#define g_P1Slot	(*(PTR(BigSprite) *)g_SceneVar3B28)	/* scene variable: player 1's select slot */
#define g_P2Slot	(*(PTR(BigSprite) *)g_SceneVar3B2C)	/* scene variable: player 2's select slot */

void PlayerSelect_CheckDone(void)
{
	if (!g_SceneExitTimer) {
		if (g_P2Slot->charIndex == -1 && g_P1Slot->confirmed)
			g_SceneExitTimer = 2;
		else if (g_P1Slot->charIndex == -1 && g_P2Slot->confirmed)
			g_SceneExitTimer = 2;
		else if (g_P1Slot->confirmed && g_P2Slot->confirmed)
			g_SceneExitTimer = 2;
	}
}
