/* PlayerSelect_NextFreeChar - the character after c that neither player's slot holds, wrapping
 * from the last to the first (Right on the player select).
 *
 * c: the current character (-1: none, returned as is).
 * Returns c itself when every other character is taken. */
#include "xmen2.h"

#define g_SelectCount	(*(s16 *)g_SceneVar3B0E)	/* scene variable: selectable characters (6, or 7 with Magneto) */
#define g_P1Slot	(*(PTR(BigSprite) *)g_SceneVar3B28)	/* scene variable: player 1's select slot */
#define g_P2Slot	(*(PTR(BigSprite) *)g_SceneVar3B2C)	/* scene variable: player 2's select slot */
#if defined(XMEN2_NATIVE)	/* phase 7: and in no player 3-4 slot */
#define FREE_CHAR(i)	(i != g_P1Slot->charIndex && i != g_P2Slot->charIndex && P34_CharFree(i))
#else
#define FREE_CHAR(i)	(i != g_P1Slot->charIndex && i != g_P2Slot->charIndex)
#endif

short PlayerSelect_NextFreeChar(register short c)
{
	register short i;

	if (c != -1) {
		for (i = c + 1; i < g_SelectCount; ++i) {
			if (FREE_CHAR(i))
				return i;
		}
		for (i = 0; i < c; ++i) {
			if (FREE_CHAR(i))
				return i;
		}
	}
	return c;
}
