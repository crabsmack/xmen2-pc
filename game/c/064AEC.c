/* PlayerSelect_PlaceCursor - put a slot's cursor on the portrait frame of its character (the
 * scene's table of frame positions); with no valid character the cursor goes off screen (-200,
 * -200) and the character becomes -1.
 *
 * slot: the player's select slot. */
#include "xmen2.h"

#define g_SelectCount	(*(s16 *)g_SceneVar3B0E)	/* scene variable: selectable characters (6, or 7 with Magneto) */
#define g_SelectPos	(*(PTR(SelectPos) *)&g_SceneVar3B30)	/* scene variable: their portrait frames' screen positions */

void PlayerSelect_PlaceCursor(register BigSprite *slot)
{
	register SelectPos *pos = g_SelectPos;

	if (slot->charIndex < 0 || slot->charIndex >= g_SelectCount) {
		slot->cursorX = -200;
		slot->cursorY = -200;
		slot->charIndex = -1;
	} else {
		slot->cursorX = pos[slot->charIndex].x;
		slot->cursorY = pos[slot->charIndex].y;
	}
}
