/* Pause_LinkSprites - while paused, end the sprite list after the PAUSE text and the players:
 * the text's second sprite links to slot 4 with both players alive, to slot 2 with one (its old
 * link saved for Pause_RestoreLink); nothing when it ended the list already. */
#include "xmen2.h"

#define g_PauseSavedLink	g_TextWinRight	/* reused: the text sprite's link before the pause */

void Pause_LinkSprites(void)
{
	if ((g_PauseSavedLink = g_SpriteTable[1].link) != 0) {
		if (IS_ALIVE(&g_Player1) && IS_ALIVE(&g_Player2))
			g_SpriteTable[1].link = 4;
		else if (IS_ALIVE(&g_Player1) || IS_ALIVE(&g_Player2))
			g_SpriteTable[1].link = 2;
	}
}
