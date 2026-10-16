/* PlayerSelect_FirstFreeChar - the first character neither player's slot holds (a player joining
 * the player select).
 *
 * Returns it, or -1 when all are taken. */
#include "xmen2.h"

#define g_SelectCount	(*(s16 *)g_SceneVar3B0E)	/* scene variable: selectable characters (6, or 7 with Magneto) */
#define g_P1Slot	(*(PTR(BigSprite) *)g_SceneVar3B28)	/* scene variable: player 1's select slot */
#define g_P2Slot	(*(PTR(BigSprite) *)g_SceneVar3B2C)	/* scene variable: player 2's select slot */

short PlayerSelect_FirstFreeChar(void)
{
	register short i;

	for (i = 0; i < g_SelectCount; ++i) {
		if (i != g_P1Slot->charIndex && i != g_P2Slot->charIndex)
			return i;
	}
	return -1;
}
